#pragma once

#include "wrapped_gl.h"
#include "image.hpp"

void GL_TextureFilter(const GLuint& tex, GLenum min, GLenum mag);
GLenum GL_BaseFormat(int channels);
GLuint GL_CreateTexture(const Image& image);
GLuint GL_CreateTexture(GLuint width, GLuint height, GLenum format);