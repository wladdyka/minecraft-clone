//
// Created by wladdyka on 7/23/2023.
//

#include "mesh.h"

Mesh::Mesh() {
    vaoId = 0;
    vboId = 0;
    iboId = 0;
    indexCount = 0;
}

Mesh::~Mesh() {
    ClearMesh();
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numberOfVertices,
                      unsigned int numberOfIndices) {
    indexCount = numberOfIndices;

    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numberOfVertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::RenderMesh() {
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::ClearMesh() {
    if (iboId != 0) {
        glDeleteBuffers(1, &iboId);
        iboId = 0;
    }

    if (vboId != 0) {
        glDeleteBuffers(1, &vboId);
        vboId = 0;
    }

    if (vaoId != 0) {
        glDeleteVertexArrays(1, &vaoId);
        vaoId = 0;
    }

    indexCount = 0;
}
