#ifndef CUSTOMSPHERE_H
#define CUSTOMSPHERE_H

#include "graphicsobject.h"

class CustomSphere : public GraphicsObject
{
public:
    CustomSphere(float radius, int stacks=20, int slices=20);

protected:
    virtual void drawGeometry();

private:
    float _radius;
    int _stacks;
    int _slices;

};

#endif // CUSTOMSPHERE_H
