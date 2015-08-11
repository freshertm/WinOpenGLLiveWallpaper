#include "graphicscene.h"

GraphicScene::GraphicScene()
{
}

void GraphicScene::addLight(GLLight * light)
{
    _lights.push_back(light);
}

/*

GLModel *GraphicScene::addRenderable(const SceneModel &obj)
{
    GLModel * model = new GLModel(obj);
    _models.push_back(model);
    return model;
}
*/
void GraphicScene::addRenderable(Renderable * model)
{
    _renderables.push_back(model);
}

GLLight *GraphicScene::light(int lightNo) const
{
    if (lightNo<lightCount())
        return _lights[lightNo];
    return 0;
}

Renderable *GraphicScene::renderable(int renderableNo) const
{
    if (renderableNo < _renderables.size())
        return _renderables[renderableNo];
    return 0;
}

int GraphicScene::lightCount() const
{
    return _lights.size();
}

int GraphicScene::renderableCount() const
{
    return _renderables.size();
}
