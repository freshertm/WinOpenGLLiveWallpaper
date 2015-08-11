#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

//#include "vec4.h"
#include "3dmath.h"

class GraphicsContext
{
public:
    GraphicsContext(const vec4 &cameraPos);
    vec4 cameraPos() const;

private:
    vec4 _cameraPos;
};
#endif // GRAPHICSCONTEXT_H
