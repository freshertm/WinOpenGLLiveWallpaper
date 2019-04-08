#include "GLEarth.h"

GLEarth::GLEarth() : GLWindow(), _player(new Player), _render(new GLRender)
{
}

GLEarth::~GLEarth()
{
};

void GLEarth::initGL()
{
    _render->init();
    _player->createScene(_render);
}

void GLEarth::resizeGL(int width, int height)
{
    _render->resize(width, height);
}

void GLEarth::drawGL()
{
    _player->step();
    _render->beginFrame();
    _render->renderScene(_player->scene());
    _render->endFrame();
}