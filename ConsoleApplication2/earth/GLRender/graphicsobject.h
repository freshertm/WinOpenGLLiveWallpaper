#ifndef _GRAPHICSOBJECT_H
#define _GRAPHICSOBJECT_H

#include "renderable.h"
#include <vector>
#include "glmaterial.h"
//#include <QGLShaderProgram>

class GraphicsObject: public Renderable
{
public:
    GraphicsObject();
    virtual ~GraphicsObject();

    vec4 rotate;
    vec4 scale;

    void setMaterial(const GLMaterial & material);
    GLMaterial material();

    //void setShader(QGLShaderProgram *shader);
    //QGLShaderProgram * shader();

    void render(const GraphicsContext & context);

    void addChild(GraphicsObject *);

protected:
    virtual void transform();
    virtual void prepareMaterial();
    //virtual void bindShader(QGLShaderProgram &_shader, const GraphicsContext& context);

    virtual void drawGeometry();

    virtual void releaseShader();
    virtual void releaseMaterial();

private:
    typedef std::vector<GraphicsObject *> object_vector_t;
    object_vector_t _childs;
    GLMaterial _material;
    //QGLShaderProgram *_shader;
};

#endif // GRAPHICSOBJECT_H
