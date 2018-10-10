//
//  PlayerView.h
//  Player
//
//  Created by multimedia on 2018/10/10.
//  Copyright © 2018年 ___multiMedia___. All rights reserved.
//

#ifndef PlayerView_h
#define PlayerView_h

#import <UIKit/UIKit.h>

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@interface PlayerView : UIView
{
    CAEAGLLayer* _eaglLayer;
    EAGLContext* _context;
    GLuint _colorRenderBuffer;
    GLuint _positionSlot;
    GLuint _colorSlot;
    GLuint _projectionUniform;
    GLuint _modelViewUniform;
    float _currentRotation;
    GLuint _depthRenderBuffer;
    
    GLuint _vertexPosition;
    GLuint _pixelColor;
    GLuint _programHandle;
}
@end
#endif /* PlayerView_h */
