#ifndef _SHADER_H_
#define _SHADER_H_

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#ifdef __cplusplus

class Shader
{
public:
    Shader();
    const char *vertex_str();
    const char *fragment_str();
    GLuint get_handle();
private:
    int compile();
public:
  
private:
    GLuint m_program_handle;
};



extern "C"
{
    GLuint get_handle();
}

#endif


#endif
