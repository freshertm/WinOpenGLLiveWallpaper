#include "glWrap.h"



static void stub(void)
{
}

GLWrap::GLWrap()
{   
    GLint majV=0;
    GLint minV=0;
    glGetIntegerv(GL_MAJOR_VERSION, &majV);
    glGetIntegerv(GL_MINOR_VERSION, &minV);

    if (majV == 0)  // OpenGL < 3.0
    {
        const GLubyte* ver = glGetString(GL_VERSION);
        if (ver != nullptr)
            majV = ver[0] - '0';
    }

    if (majV >= 2)
        init20();
}


PROC GLWrap::getProcAddress(LPCSTR procName)
{
    PROC proc = wglGetProcAddress(procName);
    if (!proc)
        return (PROC)stub;
    return proc;
};

void GLWrap::init20()
{
    _createShader = (PFNGLCREATESHADERPROC)getProcAddress("glCreateShader");
    _deleteShader = (PFNGLDELETESHADERPROC)getProcAddress("glDeleteShader");
    
    _attachShader = (PFNGLATTACHSHADERPROC)getProcAddress("glAttachShader");
    _detachShader = (PFNGLDETACHSHADERPROC)getProcAddress("glDetachShader");
    
    _compileShader = (PFNGLCOMPILESHADERPROC)getProcAddress("glCompileShader");
    _getShaderSource = (PFNGLGETSHADERSOURCEPROC)getProcAddress("glGetShaderSource");
    _shaderSource = (PFNGLSHADERSOURCEPROC)getProcAddress("glShaderSource");
    
    _createProgram = (PFNGLCREATEPROGRAMPROC)getProcAddress("glCreateProgram");
    _deleteProgram = (PFNGLDELETEPROGRAMPROC)getProcAddress("glDeleteProgram");
}

GLuint GLWrap::createShader(GLenum type)
{
    return _createShader(type);
}
void GLWrap::deleteShader(GLuint shader)
{
    _deleteShader(shader);
}


GLuint GLWrap::createProgram()
{
    return _createProgram();
}
void GLWrap::deleteProgram(GLuint program)
{
    _deleteProgram(program);
}

void GLWrap::attachShader(GLuint program, GLuint shader)
{
    _attachShader(program, shader);
}
void GLWrap::detachShader(GLuint program, GLuint shader)
{
    _detachShader(program, shader);
}

void GLWrap::compileShader(GLuint shader)
{
    _compileShader(shader);
}
void GLWrap::getShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
{
    _getShaderSource(shader, bufSize, length, source);
}

void GLWrap::shaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
{
    _shaderSource(shader, count, string, length);
}