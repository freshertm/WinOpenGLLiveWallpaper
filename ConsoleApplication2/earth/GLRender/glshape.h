#ifndef GLSHAPE_H
#define GLSHAPE_H
#include "common.h"
#include <stdint.h>


//#include "tiny_obj_loader.h"

class GLShape
{
public:
    //GLShape(tinyobj::shape_t shape);
    GLShape();
    GLShape(const GLShape & shape);
    ~GLShape();
    void render();

private:
    GLfloat * vertices;
    GLfloat * normals;
    GLfloat * texcoords;
    GLushort   * indices;

    uint32_t verticesCount;
    uint32_t normalsCount;
    uint32_t texCoordsCount;
    uint32_t indicesCount;
};

#endif // GLSHAPE_H
