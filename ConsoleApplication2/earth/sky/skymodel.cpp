#include "skymodel.h"
#include "graphicscontext.h"
#include <qgl.h>

SkyModel::SkyModel(float skyRadius, float earthRadius, int stack_slices):
    CustomSphere(skyRadius, stack_slices, stack_slices),
    _attInFactor(3.0),_attOutFactor(3.0), _reflAttShift(0.3)
{
    _range = sqrt(skyRadius * skyRadius - earthRadius * earthRadius) / skyRadius;

    QGLShaderProgram * skyShader = new QGLShaderProgram();

    skyShader->bind();
    qDebug() << "vert: " << skyShader->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/sky.vert");
    qDebug() << "frag: " << skyShader->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/sky.frag");
    qDebug() << "link: " << skyShader->link();
    _rangeId = skyShader->uniformLocation("range");
    _attInId = skyShader->uniformLocation("attInFactor");
    _attOutId = skyShader->uniformLocation("attOutFactor");
    _camId = skyShader->uniformLocation("cameraPosition");
    _reflId = skyShader->uniformLocation("reflectAttShift");
    skyShader->release();

    setShader(skyShader);
}

void SkyModel::setAttInFactor(float attFactor)
{
    _attInFactor = attFactor;
}

void SkyModel::setAttOutFactor(float attFactor)
{
    _attOutFactor = attFactor;
}

void SkyModel::setSkyColor(const Color4 &color)
{
    GLMaterial m;
    m.setDiffuse(color);
    setMaterial(m);
}

Color4 SkyModel::color()
{
    return material().diffuse();
}

float SkyModel::attOutFactor()
{
    return _attOutFactor;
}

float SkyModel::attInFactor()
{
    return _attInFactor;
}

void SkyModel::bindShader(QGLShaderProgram &shader, const GraphicsContext &context)
{
    shader.bind();
    vec4 campos = context.cameraPos();
    shader.setUniformValue(_camId, campos.x, campos.y, campos.z );
    shader.setUniformValue(_rangeId, _range);
    shader.setUniformValue(_attInId, _attInFactor);
    shader.setUniformValue(_attOutId, _attOutFactor);
    shader.setUniformValue(_reflId, _reflAttShift);
}

void SkyModel::drawGeometry()
{
    glDisable(GL_CULL_FACE);
    CustomSphere::drawGeometry();
    glEnable(GL_CULL_FACE);
}
