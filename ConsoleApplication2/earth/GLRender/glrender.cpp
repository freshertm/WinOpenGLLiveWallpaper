#include "glrender.h"
#include "common.h"
#include "glmodel.h"
#include "graphicscontext.h"
#include "glcamera.h"
#include "graphicscene.h"

GLRender::GLRender(): _camera(0)
{
}

void GLRender::init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glClearColor(0,0,0,1);
    glColor4f(1,1,1,1);
    //glDisable(GL_ALPHA_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    //glEnable(GL_COLOR_MATERIAL);
    Color4 c(0,0,0,1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat*) & c);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLRender::resize(int width, int height)
{
    //glViewport(0,0,width,height);
    if (_camera)
        _camera->update(width,height);
}

void GLRender::beginFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glColor4f(1,1,1,1);
}

void GLRender::endFrame()
{
    glFinish();
    glFlush();
}

void GLRender::renderScene(const GraphicScene & scene)
{

    GLUquadric * quad = gluNewQuadric();
    //Set lights

    if (_camera)
        _camera->update();


    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    for (int i=0; i< GL_MAX_LIGHTS; ++i)
    {
        if (i < scene.lightCount())
        {
            GLLight * light = scene.light(i);

            glPushMatrix();
            glTranslatef(light->position().x,
                         light->position().y,
                         light->position().z);
            gluSphere(quad, 1, 30, 30);
            glPopMatrix();
        }
    }
    glEnable(GL_LIGHTING);
    gluDeleteQuadric(quad);


    glLoadIdentity();

    for (register int i=0; i< GL_MAX_LIGHTS; ++i)
    {
        register int lightid = GL_LIGHT0 + i;
        if (i < scene.lightCount())
        {
            GLLight * light = scene.light(i);

            glEnable(lightid);
            light->setup(lightid);
        }
        else glDisable(lightid);
    }


    vec4 campos;
    if (_camera)
        campos = _camera->position();
    GraphicsContext context(campos);

    for (int i=0; i<scene.renderableCount(); ++i)
        scene.renderable(i)->render(context);
}

void GLRender::setCamera(GLCamera *camera)
{
    _camera = camera;
}
