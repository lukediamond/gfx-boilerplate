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

int Image_CompSize(Image::Format fmt);
Image Image_Load(const char* path, int channels, Image::Format fmt = Image::F_U8);
Image Image_Create(uint32_t width, uint32_t height, int channels, Image::Format fmt = Image::F_U8);
void Image_Free(Image& img);