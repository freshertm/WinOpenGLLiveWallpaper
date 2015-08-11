#include "3dmath.h"

CQuaternion CQuaternion::operator*(const CQuaternion &q) const
{
    vec3 v  = vec3(x, y, z);
    vec3 v1 = vec3(q.x, q.y, q.z);

    vec3 vr = (v^v1)+v1*w+v*q.w;

    return CQuaternion( vr.x, vr.y, vr.z, w*q.w - (v|v1));
}


CQuaternion CQuaternion::operator-() const
{
    return CQuaternion(-x, -y, -z, -w);
}

CQuaternion::CQuaternion(const vec3 &v, const float f)
{
    SetAxisAngle(v, f);
}

void CQuaternion::operator=(const vec3 &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 0;
}

void CQuaternion::operator=(const CQuaternion &q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
}


void CQuaternion::operator=(const float *p)
{
    x = p[0];
    y = p[1];
    z = p[2];
    w = p[3];
}

CQuaternion CQuaternion::operator+(const CQuaternion &q) const
{
    return CQuaternion(x+q.x, y+q.y, z+q.z, w+q.w);
}

CQuaternion CQuaternion::operator-(const CQuaternion &q) const
{
    return CQuaternion(x-q.x, y-q.y, z-q.z, w-q.w);
}

void CQuaternion::operator*=(const CQuaternion &q)
{
    *this = *this * q;
}

void CQuaternion::operator-=(const CQuaternion &q)
{
    x-=q.x;
    y-=q.y;
    z-=q.z;
    w-=q.w;
}

void CQuaternion::operator+=(const CQuaternion &q)
{
    x+=q.x;
    y+=q.y;
    z+=q.z;
    w+=q.w;
}

float CQuaternion::MagnitudeSquared() const
{
    return x*x + y*y + z*z + w*w;
}

float CQuaternion::Magnitude() const
{
    return sqrtf(MagnitudeSquared());
}

void CQuaternion::Normalize()
{
    *this /= Magnitude();
}

CQuaternion CQuaternion::Conjugate() const
{
    return CQuaternion(-x, -y, -z, w);
}

CQuaternion CQuaternion::Inverse() const
{
    return Conjugate() / MagnitudeSquared();
}

CQuaternion CQuaternion::UnitInverse() const
{
    return Conjugate();
}

vec3 CQuaternion::RotateVector(const vec3 &v) const
{
    CQuaternion r = (*this) * CQuaternion(v);
    return r * Inverse();
}

void CQuaternion::SetAxisAngle(const vec3 &vAxis, const float fAngle)
{
    // 4 muls, 2 trig function calls
    float f = fAngle * 0.5f;
    *this = vAxis * sinf(f);
    w = cosf(f);
}

void CQuaternion::GetAxisAngle(vec3 &vAxis, float &fAngle) const
{
    // 4 muls, 1 div, 2 trig function calls
    fAngle = acosf(w);
    vAxis = *this / sinf(fAngle);
    fAngle *= 2.0f;
}

void CQuaternion::Rotate(const CQuaternion &q)
{
    *this = q * *this;
}

void CQuaternion::Rotate(const vec3 &vAxis, const float fAngle)
{
    CQuaternion q;
    q.SetAxisAngle(vAxis, fAngle);
    Rotate(q);
}

void CQuaternion::Rotate(const vec3 &vFrom, const vec3 &vTo)
{
    vec3 vAxis = vFrom ^ vTo;
    vAxis.Normalize();
    float fCos = vFrom | vTo;
    Rotate(vAxis, acosf(fCos));
}

vec3 CQuaternion::GetViewAxis() const
{
    // 6 muls, 7 adds
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xz = x * z2;
    float yy = y * y2, yz = y * z2;
    float wx = w * x2, wy = w * y2;
    return -vec3(xz+wy, yz-wx, 1-(xx+yy));
}

vec3 CQuaternion::GetUpAxis() const
{
    // 6 muls, 7 adds
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2;
    float yz = y * z2, zz = z * z2;
    float wx = w * x2, wz = w * z2;
    return vec3(xy-wz, 1-(xx+zz), yz+wx);
}

vec3 CQuaternion::GetRightAxis() const
{
    // 6 muls, 7 adds
    float /*x2 = x + x,*/ y2 = y + y, z2 = z + z;
    float xy = x * y2, xz = x * z2;
    float yy = y * y2, zz = z * z2;
    float wy = w * y2, wz = w * z2;
    return vec3(1-(yy+zz), xy+wz, xz-wy);
}
