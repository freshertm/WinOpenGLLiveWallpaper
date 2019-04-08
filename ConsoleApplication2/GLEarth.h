#pragma once

#include "GLWindow.h"
#include "player.h"
#include "glrender.h"

class GLEarth : public GLWindow
{
public:
    GLEarth();
    virtual ~GLEarth();

protected:
    virtual void initGL();
    virtual void resizeGL(int width, int height);
    virtual void drawGL();

private:
    Player *_player = nullptr;
    GLRender *_render;
};