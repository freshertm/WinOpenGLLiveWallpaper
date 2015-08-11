#include "glstarseffect.h"

#include "stdlib.h"
#include "common.h"
#include <stdint.h>

#pragma pack (push, 1)
struct svec3
{
    float x,y,z;
};
#pragma pack (pop)

GLStarsEffect::GLStarsEffect(unsigned long num, vec4 position, vec4 size): _starsCount(num)
{
    _stars = new svec3[num];
    _starsColor = new Color4[num];

    for (unsigned long int i = 0; i< num; ++i)
    {
        _stars[i].x =  position.x - size.x/2.0 + (size.x * (float)(rand())) / (float)(RAND_MAX);
        _stars[i].y =  position.y - size.y/2.0 + (size.y * (float)(rand())) / (float)(RAND_MAX);
        _stars[i].z =  position.z - size.z/2.0 + (size.z * (float)(rand())) / (float)(RAND_MAX);

        float luminocity = (float)(rand()) / (float)(RAND_MAX);
        _starsColor[i].r = luminocity;
        _starsColor[i].g = luminocity;
        _starsColor[i].b = luminocity;
        _starsColor[i].a = 1.0;
    }
}

void GLStarsEffect::render(const GraphicsContext &)
{
    glDisable(GL_LIGHTING);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, _stars);
    glColorPointer(4, GL_FLOAT, 0, _starsColor);

    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    glDrawArrays(GL_POINTS, 0, _starsCount);

    glPopMatrix();

    glEnable(GL_LIGHTING);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
