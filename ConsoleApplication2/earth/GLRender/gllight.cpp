#include "gllight.h"
#include "common.h"

GLLight::GLLight(): _changed(1), _ambient(0,0,0,1), _diffuse(1,1,1,1),
    _specular(1,1,1,1), _position(0,0,0), _spotDirection(0,0,-1), _spotCutoff(180.0),
    _constantAttenuation(1.0), _linearAttenuation(0), _quadricAttenuation(0)

{
}

vec4 GLLight::position() const
{
    return _position;
}

vec4 GLLight::spotDirection() const
{
    return _spotDirection;
}

Color4 GLLight::diffuse() const
{
    return _diffuse;
}

Color4 GLLight::ambient() const
{
    return _ambient;
}

Color4 GLLight::specular() const
{
    return _specular;
}

bool GLLight::isChanged() const
{
    return _changed;
}

void GLLight::setNoChanged()
{
    _changed = 0;
}

void GLLight::setPosition(const vec4 &pos)
{
    _position = pos;
    _changed = 1;
}

void GLLight::spotDirection(const vec4 &spotDir)
{
    _spotDirection = spotDir;
    _changed = 1;
}

void GLLight::diffuse(const Color4 & color)
{
    _diffuse = color;
    _changed = 1;
}

void GLLight::ambient(const Color4 & color)
{
    _ambient = color;
    _changed = 1;
}

void GLLight::specular(const Color4 &color)
{
    _specular = color;
    _changed = 1;
}

void GLLight::setup(int lightid)
{
    if (isChanged())
    {
        glLightfv(lightid,GL_AMBIENT, (GLfloat*)&_ambient);
        glLightfv(lightid,GL_DIFFUSE, (GLfloat*)&_diffuse);
        glLightfv(lightid,GL_SPECULAR, (GLfloat*)&_specular);

        vec4 v = _position;
        v.w = 1.0;
        glLightfv(lightid,GL_POSITION, (GLfloat*)&v);
        glLightfv(lightid,GL_SPOT_DIRECTION, (GLfloat*)&_spotDirection);
    }
}
