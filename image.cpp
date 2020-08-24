#include "image.hpp"
#include "stb_image_write.h"

int Image_CompSize(Image::Format fmt) {
    switch (fmt) {
        case Image::F_U8: return 1;
        case Image::F_F32: return 4;
    }
    return 0;
}

Image Image_Load(const char* path, int channels, Image::Format fmt) {
    Image img {};
    int w, h, c;
    stbi_set_flip_vertically_on_load(1); // flip images for OpenGL
    switch (fmt) {
        case Image::F_U8:
            img.data = stbi_load(path, &w, &h, &c, channels);
            break;
        case Image::F_F32:
            img.data = stbi_loadf(path, &w, &h, &c, channels);
            break;
    }

    img.width = (uint32_t) w;
    img.height = (uint32_t) h;
    img.pitch = (uint32_t) (img.width * channels * Image_CompSize(fmt));
    img.channels = (uint8_t) channels;
    img.format = fmt;

    return img;
}

Image Image_Create(uint32_t width, uint32_t height, int channels, Image::Format fmt) {
    Image img;
    int compsz = Image_CompSize(fmt);
    img.width = width;
    img.height = height;
    img.pitch = width * compsz * channels;
    img.channels = channels;
    img.format = fmt;
    img.data = malloc(img.pitch * img.height);

    return img;
}

void Image_WriteBMP(const Image& img, const char* path) {
    stbi_write_bmp(path, img.width, img.height, img.channels, img.data);
}

void Image_Free(Image& img) {
    free(img.data);
}