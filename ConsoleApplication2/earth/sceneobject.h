#ifndef GRAPHICOBJECT_H
#define GRAPHICOBJECT_H

//#include "tiny_obj_loader.h"
#include <vector>

class RenderDataCommon{};

class SceneObject
{
public:
    SceneObject();
    virtual ~SceneObject(){}

    RenderDataCommon * renderData;
private:
};

#endif // GRAPHICOBJECT_H
