#ifndef GLRENDER_H
#define GLRENDER_H

#include "sceneobject.h"
//#include "graphicscene.h"

class QGLShaderProgram;
class GLCamera;
class GraphicScene;
class GraphicsContext;

class GLRender
{
public:
    GLRender();

    void init();
    void resize(int width, int height);

    void beginFrame();

    void endFrame();

    void renderScene(const GraphicScene &);

    void setCamera(GLCamera *camera);

    void setShader(QGLShaderProgram *_lighting);

protected:
    GLCamera *_camera;
};


#endif // GLRENDER_H
