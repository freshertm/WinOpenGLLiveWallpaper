#ifndef GLMATERIAL_H
#define GLMATERIAL_H

#include "color.h"

struct Bitmap
{
    unsigned char* data;
    int width;
    int height;
    enum Format {
        RGB,
        RGBA
    };
    Format format;
    Bitmap(): width(0), height(0), format(RGB), data(0){}
    ~Bitmap(){ if (data) delete[] data;}
};

class GLMaterial
{
public:
    GLMaterial();

    void setTexture(const Bitmap & bitmap);

    void setDiffuse(const Color4& diffuse);
    void setAmbient(const Color4& ambient);
    void setSpecular(const Color4& specular);
    void setEmission(const Color4& emission);
    void setShininess(float shininess);

    Color4 diffuse();
    Color4 ambient();
    Color4 specular();
    Color4 emission();
    float shininess();

    void bind();
    void unbind();
private:
    Color4 _diffuse;
    Color4 _ambient;
    Color4 _specular;
    Color4 _emission;
    float  _shininess;

    unsigned int _texId;
};

#endif // GLMATERIAL_H
