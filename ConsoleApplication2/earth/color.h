#ifndef COLOR_H
#define COLOR_H


#pragma pack (push, 1)

struct Color3{
    float r;
    float g;
    float b;
    Color3(float r=0.0, float g=0.0, float b=0.0): r(r), g(g), b(b){}
};

struct Color4{
    float r;
    float g;
    float b;
    float a;
    Color4(float r=0.0,float g=0.0,float b=0.0,float a=1.0):r(r), g(g), b(b) , a(a){}
};

#pragma pack (pop)

#endif // COLOR_H

