#ifndef GLSTARSEFFECT_H
#define GLSTARSEFFECT_H

#include "color.h"
#include "renderable.h"

struct svec3;
class GLStarsEffect: public Renderable
{
public:
    GLStarsEffect(unsigned long num, vec4 position, vec4 size);

    virtual void render(const GraphicsContext&);

private:
    svec3 * _stars;
    Color4 * _starsColor;
    unsigned long _starsCount;
};

#endif // GLSTARSEFFECT_H
