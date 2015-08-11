#ifndef GLMODEL_H
#define GLMODEL_H

#include "graphicsobject.h"
#include "scenemodel.h"
#include "glshape.h"

class GLModel : public GraphicsObject
{
public:
    GLModel(const SceneModel& obj);
    GLModel(const GLModel & model);
    GLModel();

protected:
    virtual void drawGeometry();

private:
    std::vector<GLShape*> shapes;

};

#endif // GLMODEL_H
