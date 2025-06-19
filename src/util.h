#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

static inline uint8_t binsearch32(uint32_t word) {
    uint8_t bit;
    bit += word&0xFFFFU ? 0 : 16;
    bit += word&(0xFFU<<bit) ? 0 : 8;
    bit += word&(0xFU<<bit) ? 0 : 4;
    bit += word&(0x3U<<bit) ? 0 : 2;
    bit += word&(0x1U<<bit) ? 0 : 1; 
    return bit;
}

#endif