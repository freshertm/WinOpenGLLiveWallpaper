#include "glmaterial.h"
#include "common.h"

GLMaterial::GLMaterial(): _diffuse(Color4(0.8f, 0.8f, 0.8f, 1.0f)),
                        _ambient(  Color4(0.2f, 0.2f, 0.2f, 1.0f)),
                        _specular( Color4(0.0f, 0.0f, 0.0f, 1.0f)),
                        _emission( Color4(0.0f, 0.0f, 0.0f, 1.0f)),
                        _shininess(0.0f), _texId(0)
{
}

void GLMaterial::setTexture(const Bitmap &bitmap)
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &_texId);
    glBindTexture(GL_TEXTURE_2D, _texId);
    int format = (bitmap.format == Bitmap::RGB)? GL_RGB: GL_RGBA;
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bitmap.width, bitmap.height, format, GL_UNSIGNED_BYTE, bitmap.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glDisable(GL_TEXTURE_2D);
}

void GLMaterial::setDiffuse(const Color4 &diffuse)
{
    _diffuse = diffuse;
}

void GLMaterial::setAmbient(const Color4 &ambient)
{
    _ambient = ambient;
}

void GLMaterial::setSpecular(const Color4 &specular)
{
    _specular = specular;
}

void GLMaterial::setEmission(const Color4 &emission)
{
    _emission = emission;
}

void GLMaterial::setShininess(float shininess)
{
    _shininess = shininess;
}

Color4 GLMaterial::diffuse()
{
    return _diffuse;
}

Color4 GLMaterial::ambient()
{
    return _ambient;
}

Color4 GLMaterial::specular()
{
    return _specular;
}

Color4 GLMaterial::emission()
{
    return _emission;
}

float GLMaterial::shininess()
{
    return _shininess;
}

void GLMaterial::bind()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat*)&_emission);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, _shininess);

    if (_texId)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _texId);
    }
}

void GLMaterial::unbind()
{
    if (_texId){
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}
