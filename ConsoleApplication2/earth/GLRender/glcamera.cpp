#include "glcamera.h"
#include <qgl.h>
#include <GL/glu.h>

GLCamera::GLCamera(): _isChanged(true), _width(1), _height(1),_up(0,1,0)
{
}

vec3 GLCamera::position() const
{
    return _position;
}

void GLCamera::setPosition(const vec3& position)
{
    _position = position;
}

vec3 GLCamera::lookPoint() const
{
    return _lookTo;
}

void GLCamera::setLookPoint(const vec3& lookTo)
{
    _lookTo = lookTo;
}

void GLCamera::setOrtho(const Projection &proj)
{
    _type = Ortho;
    _projection = proj;
    _isChanged = true;
}

void GLCamera::setFrustum(const Projection &proj)
{
    _type = Frustum;
    _projection = proj;
    _isChanged = true;
}

void GLCamera::setUp(const vec3 &up)
{
    _up = up;
}

vec3 GLCamera::upVector()
{
    return _up;
}

void GLCamera::setFrustum(float fov, float znear, float zfar)
{
    _type = FrustumFOV;
    _fov = fov;
    _projection.znear = znear;
    _projection.zfar = zfar;
    _isChanged = true;
}

void GLCamera::update(float screenWidth, float screenHeight)
{
    _width = screenWidth;
    _height = screenHeight;
    _isChanged = true;
    update();
}

void GLCamera::update()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    switch (_type){
        case Frustum:
            glFrustum(_projection.left,_projection.right,
                  _projection.bottom, _projection.top,
                  _projection.znear,  _projection.zfar);
            break;
    case FrustumFOV:
    {
        Projection p;
            p.znear = _projection.znear;
            p.zfar  = _projection.zfar;
            p.right =  p.znear * tan(_fov * 3.1415926 / 180.0);
            p.left = -p.right;
            p.bottom = p.right * _height / _width;
            p.top    = -p.bottom;
            glFrustum(p.left,p.right,
                  p.bottom, p.top,
                  p.znear,  p.zfar);
    }
        break;
    case Ortho:
        glOrtho(_projection.left,_projection.right,
                _projection.bottom, _projection.top,
                _projection.znear,  _projection.zfar);
        break;
    }

    gluLookAt(_position.x, _position.y, _position.z,
              _lookTo.x, _lookTo.y, _lookTo.z,
              _up.x, _up.y, _up.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _isChanged = false;
}
