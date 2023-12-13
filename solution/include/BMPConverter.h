#ifndef CONVERTER_OF_BMP
#define CONVERTER_OF_BMP

#include <stdio.h>


#pragma pack(push, 1)
struct bmp_header
{
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

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
