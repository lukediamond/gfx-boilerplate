#include <iostream>

#include "shader.hpp"

GLuint GL_CreateShader(const char* src, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint elen {};
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &elen);
        char* err = new char[elen] {};
        glGetShaderInfoLog(shader, elen, nullptr, err);
        std::cerr << "Shader error:\n" << err << '\n';
        delete[] err;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint GL_CreateProgram(const GLuint* shaders, int nshaders) {
    GLuint prog = glCreateProgram();
    for (int i = 0; i < nshaders; ++i) 
        glAttachShader(prog, shaders[i]);
    glLinkProgram(prog);
    GLint status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint elen {};
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &elen);
        char* err = new char[elen] {};
        glGetProgramInfoLog(prog, elen, nullptr, err);
        std::cerr << "Program error:\n" << err << '\n';
        delete[] err;
        glDeleteProgram(prog);
        return 0;
    }
    return prog;
}

GLuint GL_CreateProgram(const char* vsrc, const char* fsrc) {
    GLuint shaders[2] = {
        GL_CreateShader(vsrc, GL_VERTEX_SHADER),
        GL_CreateShader(fsrc, GL_FRAGMENT_SHADER),
    };
    if (!shaders[0] || !shaders[1]) {
        glDeleteShader(shaders[0]);
        glDeleteShader(shaders[1]);
        return 0;
    }

    GLuint prog = GL_CreateProgram(shaders, 2);
    
    if (!prog) {
        glDeleteShader(shaders[0]);
        glDeleteShader(shaders[1]);
    }

    return prog;
}