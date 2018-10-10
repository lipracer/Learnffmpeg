#include "Shader.h"
#include <Foundation/Foundation.h>
#ifdef __cplusplus

Shader::Shader():m_program_handle(0)
{
    compile();
}

GLuint Shader::get_handle()
{
    return m_program_handle;
}

const char *Shader::vertex_str()
{
    return R"(
    #version 330
    layout (location = 0)
    in vec3 aPos;
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
    )";
}

const char *Shader::fragment_str()
{
    return R"(
    #version 330
    out vec4 FragColor;    
    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
    )";
}

int Shader::compile()
{
    unsigned int vertex = 0, fragment = 0;
    int success;
    char infoLog[512];
    
    // 顶点着色器
    const char *str_vertex = vertex_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &str_vertex, nullptr);
    glCompileShader(vertex);
    // 打印编译错误（如果有的话）
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        printf(infoLog);
    };
    
    // fragment着色器
    const char *str_fragment = fragment_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &str_fragment, nullptr);
    glCompileShader(fragment);
    // 打印编译错误（如果有的话）
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        printf(infoLog);
    };
    
    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // 打印连接错误（如果有的话）
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        printf(infoLog);
    }
    
    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    
    //glUseProgram(ID);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return 0;
}

GLuint get_handle()
{
    Shader shader;
    return shader.get_handle();
}

#endif

