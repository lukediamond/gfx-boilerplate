#pragma once

#include <GL/glew.h>

GLuint GL_CreateShader(const char* src, GLenum type);
GLuint GL_CreateProgram(const GLuint* shaders, int nshaders);
GLuint GL_CreateProgram(const char* vsrc, const char* fsrc);