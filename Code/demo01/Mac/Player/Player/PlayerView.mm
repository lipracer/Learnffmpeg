//
//  PlayerView.m
//  Player
//
//  Created by multimedia on 2018/10/10.
//  Copyright © 2018年 ___multiMedia___. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PlayerView.h"
#include "Shader.h"

typedef struct {
    float Position[3];
    float Color[4];
} Vertex;

const float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

const GLubyte Indices[] = {
    0, 1, 2,
    2, 3, 0
};


@implementation PlayerView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)setupLayer {
    _eaglLayer = (CAEAGLLayer*) self.layer;
    _eaglLayer.opaque = YES;
}

- (void)setupContext {
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        exit(1);
    }
    
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        exit(1);
    }
}

- (void)setupRenderBuffer {
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
}

- (void)setupFrameBuffer {
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);
    
    
    // Add to end of setupFrameBuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
}


// Add new method right after setupRenderBuffer
- (void)setupDepthBuffer {
    glGenRenderbuffers(1, &_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, self.frame.size.width, self.frame.size.height);
}

//- (void)setupVBOs {
//    //顶点VBO
//    GLuint vertexBuffer;
//    glGenBuffers(1, &vertexBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    //将顶点坐标写入顶点VBO
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    //索引VBO
//    GLuint indexBuffer;
//    glGenBuffers(1, &indexBuffer);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
//    //将顶点索引数据写入索引VBO
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
//}
//
//-(void)configuerSlot{
//    //获取shader中attribute变量的引用
//    _vertexPosition = glGetAttribLocation(_programHandle, "vertexPosition");
//    _pixelColor = glGetAttribLocation(_programHandle, "pixelColor");
//    //启用attribute变量，使其对GPU可见，默认为关闭
//    glEnableVertexAttribArray(_vertexPosition);
//    glEnableVertexAttribArray(_pixelColor);
//}


//- (void) feedAttributeSlot{
//        //由于使用了VBO，所以最后一个参数传数据在VBO中的偏移量，这点需要注意
//        glVertexAttribPointer(_vertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
//        //没有使用VBO，直接传指针给函数,
//        float blueColor[] = {0, 0, 1, 0};
//        glVertexAttribPointer(_pixelColor, 4, GL_FLOAT, GL_FALSE, 0, blueColor);
//}

- (void)render {
    glClearColor(0, 104.0/255.0, 55.0/255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 1
    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
    
    // 2
    glDrawArrays(GL_TRIANGLES, 0, 3);
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

// Replace initWithFrame with this
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self setupLayer];
        [self setupContext];
    
        [self setupDepthBuffer];
        [self setupRenderBuffer];
        [self setupFrameBuffer];
        
        
        
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 1. 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // 2. 当我们渲染一个物体时要使用着色器程序
        _programHandle = get_handle();
        glUseProgram(_programHandle);
        // 3. 绘制物体

    }
    return self;
}

- (void)dealloc
{
    _context = nil;
}

@end

