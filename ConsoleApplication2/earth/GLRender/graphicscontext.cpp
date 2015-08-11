#include "graphicscontext.h"

GraphicsContext::GraphicsContext(const vec4 &cameraPos): _cameraPos(cameraPos)
{
}


vec4 GraphicsContext::cameraPos() const
{
    return _cameraPos;
}
