#pragma once

#include <cstdint>
#include <cstddef>

#define UTF8_INVALID_CHAR ((uint32_t) 0xffffffffffffffff)

inline int UTF8_PrefixLen(char x) {
    if (x >= 0) return 1;
    #ifdef __builtin_clz
    return __builtin_clz((uint32_t) ~x << 24);
    #else
    int n = 0;
    while (x < 0) {
        x <<= 1;
        ++n;
    }
    return n;
    #endif
}

inline uint32_t UTF8_NextCodePoint(const char* buf, size_t* i, size_t len) {
    const int preflen = UTF8_PrefixLen(buf[*i]);
    // insufficient buffer size
    if (preflen > len - *i) {
        *i = len;
        return UTF8_INVALID_CHAR;
    }

    uint32_t codept = (uint8_t) buf[*i] & ((uint8_t) 0xff >> preflen);
    for (int j = 1; j < preflen; ++j) {
        codept <<= 6;
        codept |= (uint8_t) buf[*i + j] & 63;
    }
    *i += preflen;
    return codept;
}