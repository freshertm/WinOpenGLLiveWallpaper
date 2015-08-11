#ifndef RENDERABLE_H
#define RENDERABLE_H
//#include "vec4.h"

#include "3dmath.h"

typedef TVector4<float> vec4;


class GraphicsContext;
class Renderable{
public:
    Renderable(){}
    virtual ~Renderable(){}

    vec4 position;

    virtual void render(const GraphicsContext &conext) = 0;
};

#endif // GLEFFECT_H
