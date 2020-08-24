#pragma once

#include <string>
#include <fstream>

inline std::string ReadEntireFile(const char* path) {
    std::ifstream stream(path);
    if (!stream.is_open()) return "";
    stream.seekg(0, std::ios::end);
    size_t sz = stream.tellg();
    stream.seekg(0);
    std::string data(sz, 0);
    stream.read(&data[0], sz);
    stream.close();
    return data;
}