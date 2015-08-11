#include "glmodel.h"
#include <scenemodel.h>
#include "graphicscontext.h"

GLModel::GLModel(const SceneModel &obj): GraphicsObject()
{
    for(uint32_t i =0; i< obj.shapes.size(); ++i)
    {
        tinyobj::shape_t shp = obj.shapes[i];
        try {
            GLShape * shape = new GLShape(shp);
            shapes.push_back(shape);
        }
        catch(...)
        {
            qDebug() << "exception during shape loading";
        }
    }
}

GLModel::GLModel(const GLModel &model): GraphicsObject(model)
{
    position = model.position;
    rotate = model.rotate;
    scale = model.scale;
    for (int i=0; i<model.shapes.size(); ++i)
    {
        GLShape *shape = new GLShape(*model.shapes[i]);
        shapes.push_back(shape);
    }
}

GLModel::GLModel()
{
}

void GLModel::drawGeometry()
{
    foreach(GLShape* shape, shapes)
        shape->render();
}
