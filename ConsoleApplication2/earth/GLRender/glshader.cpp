#include "glshader.h"
#include "common.h"

#include "../glwrap.h"

GLShader::GLShader()
{
}


bool GLShader::attachVertexProgram(const std::string &programText)
{
    return false; 
}

bool GLShader::attachFragmentProgram(const std::string &programText)
{
    return false;
}
bool GLShader::linkProgram()
{
    return false;
}

void GLShader::bind()
{
}

void GLShader::unBind()
{
}