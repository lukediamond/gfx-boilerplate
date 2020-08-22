#include "gl_texture.hpp"

void GL_TextureFilter(const GLuint& tex, GLenum min, GLenum mag) {
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, min);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, mag);
}

GLenum GL_BaseFormat(int channels) {
    switch (channels) {
        case 1: return GL_RED;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return GL_RED;
    }
}

GLuint GL_CreateTexture(const Image& image) {
    GLuint tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    
    GLenum type;
    switch (image.format) {
        case Image::F_U8:
            type = GL_UNSIGNED_BYTE;
            break;
        case Image::F_F32:
            type = GL_FLOAT;
            break;
    }

    GLenum format;
    switch (image.channels) {
        case 1:
            format = GL_R32F;
            break;
        case 2:
            format = GL_RG32F;
            break;
        case 3:
            format = GL_RGB32F;
            break;
        case 4:
            format = GL_RGBA32F;
            break;
        default:
            format = GL_R32F;
    }

    glTextureStorage2D(tex, 1, format, image.width, image.height);
    glTextureSubImage2D(
        tex, 0, 0, 0, image.width, image.height, 
        GL_BaseFormat(image.channels), type, image.data);

    return tex;
}

GLuint GL_CreateTexture(GLuint width, GLuint height, GLenum format) {
    GLuint tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    glTextureStorage2D(tex, 1, format, width, height);
    return tex;
}