#include "prim.hpp"

const GLfloat kQuadVerts[] = {
    -1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, +1.0f, 0.0f, 1.0f,
    +1.0f, +1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    +1.0f, +1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, 1.0f, 0.0f
};

Primative GL_CreateQuad() {
    Primative prim;
    glCreateVertexArrays(1, &prim.vao);
    glCreateBuffers(1, &prim.vbo);

    glBindVertexArray(prim.vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, prim.vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, GL_Offset(sizeof(GLfloat) * 0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, GL_Offset(sizeof(GLfloat) * 2));
    glBufferData(GL_ARRAY_BUFFER, sizeof(kQuadVerts), kQuadVerts, GL_STATIC_DRAW);

    prim.elements = 6;
    return prim;
}

void GL_DestroyPrimative(Primative& prim) {
    glDeleteVertexArrays(1, &prim.vao);
    glDeleteBuffers(1, &prim.vbo);
}