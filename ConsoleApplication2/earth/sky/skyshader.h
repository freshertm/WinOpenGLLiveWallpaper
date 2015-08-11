#ifndef SKYSHADER_H
#define SKYSHADER_H

#include <QGLShaderProgram>

/* range - параметр, который определяет границу земли.
 * вычисляется по формуле:
 *   range = sqrt(skyRadius * skyRadius - earthRadius * earthRadius) / skyRadius;
 *
 *
*/


class SkyShader : public QGLShaderProgram
{
public:
    SkyShader(float range, float attFactor = 7.0);
};

#endif // SKYSHADER_H
