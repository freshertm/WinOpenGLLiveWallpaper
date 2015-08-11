#include "glshape.h"
#include "common.h"

//GLShape::GLShape(tinyobj::shape_t shape): indices(0), vertices(0), normals(0), texcoords(0), indicesCount(0), verticesCount(0), normalsCount(0), texCoordsCount(0)
GLShape::GLShape() : indices(0), vertices(0), normals(0), texcoords(0), indicesCount(0), verticesCount(0), normalsCount(0), texCoordsCount(0)
{
/*    verticesCount = shape.mesh.positions.size();
    if (verticesCount <=0) throw;
    vertices = new GLfloat[verticesCount];
    memcpy(vertices,&shape.mesh.positions[0], sizeof(float) * verticesCount);*/
   /* for (int i=0; i< verticesCount; ++i)
    {
        qDebug() << "V[" << i<< "] vertices:" << vertices[i] << " shape.positions:" << shape.mesh.positions[i];
    }*/
        //vertices[i] = shape.mesh.positions[i];*/


  /*  indicesCount = shape.mesh.indices.size();
    if (indicesCount <=0) throw;
    indices = new GLushort[indicesCount];
    //memcpy(indices, &shape.mesh.indices[0], sizeof(GLushort) * indicesCount);
    for (uint32_t i=0; i< indicesCount; ++i)
        indices[i] = shape.mesh.indices[i];


    normalsCount = shape.mesh.normals.size();
    if (normalsCount > 0)
    {
        normals = new GLfloat[normalsCount];
        memcpy(normals,&shape.mesh.normals[0],normalsCount * sizeof (float) );*/
        /*for (int i=0; i< normalsCount; ++i)
            normals[i] = shape.mesh.normals[i];*/
  //  }

  /*  texCoordsCount = shape.mesh.texcoords.size();
    if (texCoordsCount > 0)
    {
        texcoords = new float[texCoordsCount];
        memcpy(texcoords,shape.mesh.texcoords.data(),texCoordsCount * sizeof (float) );
    }*/
}

GLShape::GLShape(const GLShape &shape)
{
   /* verticesCount = shape.verticesCount;
    vertices = new GLfloat[verticesCount];
    memcpy(vertices,shape.vertices,verticesCount * sizeof(GLfloat));

    normalsCount = shape.normalsCount;
    if (normalsCount){
        normals = new GLfloat[normalsCount];
        memcpy(normals,shape.normals,normalsCount * sizeof(GLfloat));
    }

    indicesCount = shape.indicesCount;
    indices = new GLushort[indicesCount];
    memcpy(indices,shape.indices,indicesCount * sizeof(GLushort));

    texCoordsCount = shape.texCoordsCount;
    if (texCoordsCount){
        texcoords = new GLfloat[texCoordsCount];
        memcpy(texcoords,shape.texcoords,texCoordsCount * sizeof(GLfloat));
    }*/
}

GLShape::~GLShape()
{
    /*delete[] vertices;
    delete[] indices;

    if (normalsCount)
        delete[] normals;

    if (texCoordsCount)
        delete[] texcoords;*/
}

/*void GLShape::render()
{
    glBegin(GL_LINES);
    glEnd();
}
*/


void GLShape::render()
{
  /*  if (normalsCount>0)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT,0,normals);
    }
    else glEnable(GL_AUTO_NORMAL);



    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);*/

    /*glEnableClientState(GL_INDEX_ARRAY);
    glIndexPointer(GL_INT,0,indices);
*/


   /* if (texCoordsCount>0)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(texCoordsCount,GL_FLOAT,0,texcoords);
    }

    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);

    if (normalsCount>0) glDisableClientState(GL_NORMAL_ARRAY);
    else glDisable(GL_AUTO_NORMAL);

    if (texCoordsCount>0) glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/

}
