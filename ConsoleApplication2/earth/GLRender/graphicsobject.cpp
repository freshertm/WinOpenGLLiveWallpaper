#include "graphicsobject.h"
#include "common.h"
#include "graphicscontext.h"

GraphicsObject::GraphicsObject():/* _shader(0),*/ scale(1,1,1,1)
{
}

GraphicsObject::~GraphicsObject()
{
}

void GraphicsObject::render(const GraphicsContext &context)
{

    glPushMatrix();

    transform();

    prepareMaterial();

/*    if (_shader)
        bindShader(*_shader, context);*/

    drawGeometry();

   /* if (_shader)
        releaseShader();*/

    releaseMaterial();

    for (object_vector_t::iterator i = _childs.begin();
        i != _childs.end(); 
        i++)
        (*i)->render(context);

    glPopMatrix();
}

void GraphicsObject::addChild(GraphicsObject * obj)
{
    _childs.push_back(obj);
}


void GraphicsObject::transform()
{
    glScalef(scale.x, scale.y, scale.z);

    glRotatef(rotate.x, 1,0,0);
    glRotatef(rotate.y, 0,1,0);
    glRotatef(rotate.z, 0,0,1);

    glTranslatef(position.x, position.y, position.z);
}
/*

void GraphicsObject::bindShader(QGLShaderProgram &_shader, const GraphicsContext& context)
{
    _shader.bind();
    vec4 campos = context.cameraPos();
    _shader.setUniformValue("cameraPosition", campos.x, campos.y, campos.z );
}*/

void GraphicsObject::drawGeometry()
{
}

void GraphicsObject::prepareMaterial()
{
    _material.bind();
}

void GraphicsObject::releaseMaterial()
{
    _material.unbind();
}

void GraphicsObject::releaseShader()
{
    /*if (_shader)
        _shader->release();*/
}



void GraphicsObject::setMaterial(const GLMaterial &material)
{
    _material = material;
}

GLMaterial GraphicsObject::material()
{
    return _material;
}

/*void GraphicsObject::setShader(QGLShaderProgram *shader)
{
    _shader = shader;
}*/
/*
QGLShaderProgram *GraphicsObject::shader()
{
    return _shader;
}
*/