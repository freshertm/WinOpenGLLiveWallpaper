#pragma once

#include "common.h"
#include "glext.h"

static void stub(void);

class GLWrap
{
public:
    GLWrap();

    GLuint createShader(GLenum type);
    void deleteShader(GLuint shader);
   

    inline GLuint createProgram();
    inline void deleteProgram(GLuint program);
    
    inline void attachShader(GLuint program, GLuint shader);
    inline void detachShader(GLuint program, GLuint shader);

    inline void compileShader(GLuint shader);
    inline void getShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
    inline void shaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);

private:

    void init20();

    PROC getProcAddress(LPCSTR);

    PFNGLCREATESHADERPROC    _createShader =    (PFNGLCREATESHADERPROC) stub;
    PFNGLDELETESHADERPROC    _deleteShader =    (PFNGLDELETESHADERPROC) stub;

    PFNGLATTACHSHADERPROC    _attachShader  =   (PFNGLATTACHSHADERPROC) stub;
    PFNGLDETACHSHADERPROC    _detachShader =    (PFNGLDETACHSHADERPROC) stub;

    PFNGLCOMPILESHADERPROC   _compileShader =   (PFNGLCOMPILESHADERPROC)stub;
    PFNGLGETSHADERSOURCEPROC _getShaderSource = (PFNGLGETSHADERSOURCEPROC)stub;
    PFNGLSHADERSOURCEPROC    _shaderSource =    (PFNGLSHADERSOURCEPROC) stub;
                                                
    PFNGLCREATEPROGRAMPROC   _createProgram =   (PFNGLCREATEPROGRAMPROC)stub;
    PFNGLDELETEPROGRAMPROC   _deleteProgram =   (PFNGLDELETEPROGRAMPROC)stub;
};