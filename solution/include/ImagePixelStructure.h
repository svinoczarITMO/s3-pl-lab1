#ifndef IMAGE_UTIL_H
#define IMAGE_UTIL_H

#include <stdint.h>
#include <stdio.h>

struct pixel{
    uint8_t b, g, r;
};

struct image{
    uint16_t width, height;
    struct pixel* data;
};

#endif
