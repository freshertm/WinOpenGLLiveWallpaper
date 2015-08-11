#ifndef PLAYER_H
#define PLAYER_H

#include "graphicscene.h"
#include "gllight.h"
#include "glcamera.h"
#include "glrender.h"


class SkyModel;
class GLModel;
class Player
{
public:
    Player();

    void createScene(GLRender* render);
    void step();

    void key(int key);

    const GraphicScene & scene();

private:
    GLModel *loadModel(const char* filename);
    Bitmap  *loadTexture(const char* filename);
    GraphicScene _scene;

    GraphicsObject * _earth;
    SkyModel * _sky;

    Renderable * _stars;

    GLLight * _sun;

    GLCamera * _cam;

    GLRender * _render;

    float _camOffset;
    float _camRotateX;
    float _camRotateZ;

    float _sunOffset;

    vec3 camPos;
    vec3 camLookVector;
    vec3 camUpVector;

    float camHeight;
    float camVSpeed;

    void calcCameraPosition(int64_t msec);
};

#endif // PLAYER_H
