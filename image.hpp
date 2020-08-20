#pragma once

#include <cstdint>

#include "stb_image.h"

struct Image {
    enum Format {
        F_F32,
        F_U8
    };
    void* data {};
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t channels;
    Format format;
};

inline int Image_PixelSize(Image::Format fmt) {
    switch (fmt) {
        case Image::F_U8: return 1;
        case Image::F_F32: return 4;
    }
}

inline Image Image_Load(const char* path, int channels, Image::Format fmt = Image::F_U8) {
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
    img.pitch = (uint32_t) (img.width * channels * Image_PixelSize(fmt));
    img.channels = (uint8_t) channels;
    img.format = fmt;

    return img;
}

inline void Image_Free(Image& img) {
    free(img.data);
}
