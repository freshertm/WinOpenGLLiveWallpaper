#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <vector>
#include "scenemodel.h"

#include "glmodel.h"
#include "gllight.h"
#include "renderable.h"

class GraphicScene
{
public:
    GraphicScene();

    void addLight(GLLight *light);
    //GLModel *addRenderable(const SceneModel &obj);
    void addRenderable(Renderable *model);

    GLLight* light(int lightNo) const;
    Renderable * renderable(int renderableNo) const;

    int lightCount() const;
    int renderableCount() const;

private:
    std::vector<Renderable*> _renderables;
    std::vector<GLLight*> _lights;
};

#endif // GRAPHICSCENE_H
