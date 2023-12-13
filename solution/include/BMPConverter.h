#ifndef DESERIALIZE_FROM_BMP_H
#define DESERIALIZE_FROM_BMP_H

#include "ImagePixelStructure.h"
#include <stdio.h>
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
