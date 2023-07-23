//
// Created by wladdyka on 7/23/2023.
//

#ifndef OPENGLCOURSE_MESH_H
#define OPENGLCOURSE_MESH_H


#include <GL/glew.h>

class Mesh {
public:
    Mesh();
    ~Mesh();

    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numberOfVertices, unsigned int numberOfIndices);
    void RenderMesh();
    void ClearMesh();

private:
    GLuint vaoId, vboId, iboId;
    GLsizei indexCount;
};


#endif //OPENGLCOURSE_MESH_H
