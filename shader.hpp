#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

GLuint GL_CreateShader(const char* src, GLenum type);
GLuint GL_CreateProgram(const GLuint* shaders, int nshaders);
GLuint GL_CreateProgram(const char* vsrc, const char* fsrc);

template <int C, typename T> inline auto GL_VecUniform = 0;
template <int C, typename T> inline auto GL_VecUniformArgs = 0;
template <int C, int R> inline auto GL_MatUniform = 0;

// float vector
template <> constexpr inline auto*& GL_VecUniform<2, GLfloat> = glUniform2fv;
template <> constexpr inline auto*& GL_VecUniform<3, GLfloat> = glUniform3fv;
template <> constexpr inline auto*& GL_VecUniform<4, GLfloat> = glUniform4fv;
// int vector
template <> constexpr inline auto*& GL_VecUniform<2, GLint> = glUniform2iv;
template <> constexpr inline auto*& GL_VecUniform<3, GLint> = glUniform3iv;
template <> constexpr inline auto*& GL_VecUniform<4, GLint> = glUniform4iv;
// uint vector
template <> constexpr inline auto*& GL_VecUniform<2, GLuint> = glUniform2uiv;
template <> constexpr inline auto*& GL_VecUniform<3, GLuint> = glUniform3uiv;
template <> constexpr inline auto*& GL_VecUniform<4, GLuint> = glUniform4uiv;

// float vector
template <> constexpr inline auto*& GL_VecUniformArgs<1, GLfloat> = glUniform1f;
template <> constexpr inline auto*& GL_VecUniformArgs<2, GLfloat> = glUniform2f;
template <> constexpr inline auto*& GL_VecUniformArgs<3, GLfloat> = glUniform3f;
template <> constexpr inline auto*& GL_VecUniformArgs<4, GLfloat> = glUniform4f;
// int vector
template <> constexpr inline auto*& GL_VecUniformArgs<1, GLint> = glUniform1i;
template <> constexpr inline auto*& GL_VecUniformArgs<2, GLint> = glUniform2i;
template <> constexpr inline auto*& GL_VecUniformArgs<3, GLint> = glUniform3i;
template <> constexpr inline auto*& GL_VecUniformArgs<4, GLint> = glUniform4i;
// uint vector
template <> constexpr inline auto*& GL_VecUniformArgs<1, GLuint> = glUniform1ui;
template <> constexpr inline auto*& GL_VecUniformArgs<2, GLuint> = glUniform2ui;
template <> constexpr inline auto*& GL_VecUniformArgs<3, GLuint> = glUniform3ui;
template <> constexpr inline auto*& GL_VecUniformArgs<4, GLuint> = glUniform4ui;

// square matrices
template <> constexpr inline auto*& GL_MatUniform<2, 2> = glUniformMatrix2fv;
template <> constexpr inline auto*& GL_MatUniform<3, 3> = glUniformMatrix3fv;
template <> constexpr inline auto*& GL_MatUniform<4, 4> = glUniformMatrix4fv;
// 2x3 and 3x2 matrices
template <> constexpr inline auto*& GL_MatUniform<2, 3> = glUniformMatrix2x3fv;
template <> constexpr inline auto*& GL_MatUniform<3, 2> = glUniformMatrix3x2fv;
// 2x4 and 4x2 matrices
template <> constexpr inline auto*& GL_MatUniform<2, 4> = glUniformMatrix2x4fv;
template <> constexpr inline auto*& GL_MatUniform<4, 2> = glUniformMatrix4x2fv;
// 3x4 and 4x3 matrices
template <> constexpr inline auto*& GL_MatUniform<3, 4> = glUniformMatrix3x4fv;
template <> constexpr inline auto*& GL_MatUniform<4, 3> = glUniformMatrix4x3fv;

template<int C, typename T, glm::qualifier Q>
inline void GL_PassUniform(GLint loc, glm::vec<C, T, Q> vec) {
    GL_VecUniform<C, T>(loc, 1, (const T*) &vec);
}

template <int C, int R, glm::qualifier Q>
inline void GL_PassUniform(GLint loc, glm::mat<C, R, float, Q> mat) {
    GL_MatUniform<C, R>(loc, 1, GL_FALSE, (const GLfloat*) &mat);
}

template <typename T, typename... Args>
inline void GL_PassUniform(GLint loc, T first, Args... rest) {
    GL_VecUniformArgs<sizeof...(Args) + 1, T>(loc, first, rest...);
}

