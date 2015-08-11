#ifndef SKYMODEL_H
#define SKYMODEL_H

#include "customsphere.h"
#include "color.h"
#include "glmaterial.h"

class SkyModel : public CustomSphere
{
public:
    SkyModel(float skyRadius, float earthRadius, int stack_slices=20);

    void setAttInFactor(float attFactor);
    void setAttOutFactor(float attFactor);
    void setSkyColor(const Color4 &color);
    Color4 color();
    float attInFactor();
    float attOutFactor();

protected:
    virtual void bindShader(QGLShaderProgram &shader, const GraphicsContext& context);
    virtual void drawGeometry();

private:
    float _attInFactor;
    float _attOutFactor;
    float _range;    
    float _reflAttShift;

    int _rangeId;
    int _attInId;
    int _attOutId;
    int _camId;
    int _reflId;
};

#endif // SKYMODEL_H
