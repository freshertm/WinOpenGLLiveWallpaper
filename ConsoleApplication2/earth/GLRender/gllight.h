#ifndef GLLIGHT_H
#define GLLIGHT_H

//#include "vec4.h"
#include "color.h"

#include "3dmath.h"


class GLLight
{
public:
    GLLight();


    vec4 position() const;
    vec4 spotDirection() const;

    Color4 diffuse() const;
    Color4 ambient() const;
    Color4 specular() const;

    bool isChanged() const;
    void setNoChanged();

    void setPosition(const vec4& pos);
    void spotDirection(const vec4& spotDir);

    void diffuse(const Color4& color);
    void ambient(const Color4& color);
    void specular(const Color4& color);

    void setup(int lightid);

private:
    vec3 _position;
    vec3 _spotDirection;

    Color4 _diffuse;
    Color4 _ambient;
    Color4 _specular;

    float _spotExponent;
    float _spotCutoff;
    float _constantAttenuation;
    float _linearAttenuation;
    float _quadricAttenuation;

    bool _changed;
};

#endif // GLLIGHT_H
