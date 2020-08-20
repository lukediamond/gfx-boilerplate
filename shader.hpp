#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

GLuint GL_CreateShader(const char* src, GLenum type);
GLuint GL_CreateProgram(const GLuint* shaders, int nshaders);
GLuint GL_CreateProgram(const char* vsrc, const char* fsrc);

template <int C, typename T> inline auto GL_VecUniform = 0;
template <int C, int R> inline auto GL_MatUniform = 0;

// float vector
template <> inline auto*& GL_VecUniform<2, GLfloat> = glUniform2fv;
template <> inline auto*& GL_VecUniform<3, GLfloat> = glUniform3fv;
template <> inline auto*& GL_VecUniform<4, GLfloat> = glUniform4fv;
// int vector
template <> inline auto*& GL_VecUniform<2, GLint> = glUniform2iv;
template <> inline auto*& GL_VecUniform<3, GLint> = glUniform3iv;
template <> inline auto*& GL_VecUniform<4, GLint> = glUniform4iv;
// uint vector
template <> inline auto*& GL_VecUniform<2, GLuint> = glUniform2uiv;
template <> inline auto*& GL_VecUniform<3, GLuint> = glUniform3uiv;
template <> inline auto*& GL_VecUniform<4, GLuint> = glUniform4uiv;

// square matrices
template <> inline auto*& GL_MatUniform<2, 2> = glUniformMatrix2fv;
template <> inline auto*& GL_MatUniform<3, 3> = glUniformMatrix3fv;
template <> inline auto*& GL_MatUniform<4, 4> = glUniformMatrix4fv;
// 2x3 and 3x2 matrices
template <> inline auto*& GL_MatUniform<2, 3> = glUniformMatrix2x3fv;
template <> inline auto*& GL_MatUniform<3, 2> = glUniformMatrix3x2fv;
// 2x4 and 4x2 matrices
template <> inline auto*& GL_MatUniform<2, 4> = glUniformMatrix2x4fv;
template <> inline auto*& GL_MatUniform<4, 2> = glUniformMatrix4x2fv;
// 3x4 and 4x3 matrices
template <> inline auto*& GL_MatUniform<3, 4> = glUniformMatrix3x4fv;
template <> inline auto*& GL_MatUniform<4, 3> = glUniformMatrix4x3fv;

template<int C, typename T, glm::qualifier Q>
inline void GL_PassUniform(GLint loc, glm::vec<C, T, Q> vec) {
    GL_VecUniform<C, T>(loc, 1, (const T*) &vec);
}

template <int C, int R, glm::qualifier Q>
inline void GL_PassUniform(GLint loc, glm::mat<C, R, float, Q> mat) {
    GL_MatUniform<C, R>(loc, 1, GL_FALSE, (const GLfloat*) &mat);
}

