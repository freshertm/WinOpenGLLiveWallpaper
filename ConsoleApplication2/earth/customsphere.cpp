#include "customsphere.h"

#include "windows.h"
#include <GL/GLU.h>

CustomSphere::CustomSphere(float radius, int stacks, int slices): GraphicsObject(),
    _radius(radius),
    _stacks(stacks),
    _slices(slices)
{
}

void CustomSphere::drawGeometry()
{
     GLUquadric * q = gluNewQuadric();
     gluQuadricTexture(q, GL_TRUE);
     gluQuadricNormals(q, GLU_SMOOTH);

     gluSphere(q, _radius, _slices, _stacks);
     gluDeleteQuadric(q);
}
