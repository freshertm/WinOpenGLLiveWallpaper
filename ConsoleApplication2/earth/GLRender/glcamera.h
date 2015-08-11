#ifndef GLCAMERA_H
#define GLCAMERA_H

#include "3dmath.h"

struct Projection{   
    float left;
    float right;
    float bottom;
    float top;
    float znear;
    float zfar;
    Projection(float left,  float right,
               float bottom,float top,
               float znear, float zfar):
        left(left), right(right),
        bottom(bottom), top(top),
        znear(znear), zfar(zfar){}

    Projection():left(-1), right(1),
        bottom(-1), top(1),
        znear(0),zfar(1){}
};

class GLCamera
{
public:
    GLCamera();

    vec3 position() const;
    void setPosition(const vec3 &position);

    vec3 lookPoint() const;
    void setLookPoint(const vec3 &lookTo);

    void setOrtho(const Projection &proj);
    void setFrustum(const Projection &proj);

    void setUp(const vec3& up);
    vec3 upVector();

    void setFrustum(float fov, float znear, float zfar);

    void update();
    void update(float screenWidth, float screenHeight);


private:
    vec3 _position;
    vec3 _lookTo;
    vec3 _up;
    Projection _projection;
    bool _isChanged;

    enum ProjectionType {
        Ortho,
        Frustum,
        FrustumFOV
    };

    ProjectionType _type;

    float _width;
    float _height;
    float _fov;
};

#endif // GLCAMERA_H
