#pragma once

#include "wrapped_gl.h"

struct Primative {
    GLuint vao {};
    GLuint vbo {};
    GLuint elements {};

    void Draw() {
        glDrawArrays(GL_TRIANGLES, 0, elements);
    }
};

inline constexpr const GLvoid* GL_Offset(size_t offset) {
    return (const GLvoid*) (GLintptr) offset;
}

Primative GL_CreateQuad();
void GL_DestroyPrimative(Primative& prim);