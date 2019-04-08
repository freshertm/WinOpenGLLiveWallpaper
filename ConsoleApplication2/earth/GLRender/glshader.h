#ifndef GLSHADER_H
#define GLSHADER_H

#include <string>

//class GLRender;
class GLShader
{
public:
    GLShader();

    bool attachVertexProgram(const std::string &programText);
    bool attachFragmentProgram(const std::string &programText);
    bool linkProgram();

    void bind();
    void unBind();
};

#endif // GLSHADER_H
