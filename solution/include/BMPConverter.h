#ifndef CONVERTER_OF_BMP
#define CONVERTER_OF_BMP

#include <stdio.h>

struct pixel{
    uint8_t b, g, r;
};

struct image{
    uint16_t width, height;
    struct pixel* data;
};

enum read_status {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER
};

enum write_status {
    WRITE_OK = 0,
    WRITE_ERROR

};

enum write_status to_bmp(FILE* out, struct image* img);

enum read_status from_bmp(FILE* in, struct image* img);

#endif
