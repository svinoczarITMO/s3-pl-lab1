#ifndef CONVERTER_OF_BMP
#define CONVERTER_OF_BMP

#include <stdio.h>

enum write_status to_bmp(FILE* out, struct image* img);

enum read_status from_bmp(FILE* in, struct image* img);

#endif
