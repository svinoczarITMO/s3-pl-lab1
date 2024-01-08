#include "BMPConverter.h"
#include "HeaderStructure.h"
#include "ImagePixelStructure.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BfType 0x4d42
#define Zero_value 0
#define Bi_planes 1
#define Bi_size 40
#define Bit_count 24

uint8_t padding_calc(uint16_t w){
    return (uint8_t) (4 - ((w * sizeof(struct pixel)) % 4)) % 4;
}

enum read_status from_bmp(FILE* in, struct image* img)
{
    BmpHeader old;
    size_t bytesRead = fread(&old, sizeof(BmpHeader), 1, in);
    if (bytesRead != 1) {
        return READ_INVALID_HEADER;
    }

    if (old.bfType != BfType) {
        return READ_INVALID_SIGNATURE;
    }

    img->width = old.biWidth;
    img->height = old.biHeight;

    img->data = (struct pixel*)malloc(sizeof(struct pixel) * img->width * img->height);
    if (img->data == NULL) {
        return READ_INVALID_BITS;
    }

    if (fseek(in, old.bOffBits, SEEK_SET) != 0) {
        free(img->data);
        return READ_INVALID_BITS;
    }

    uint8_t paddingBytes = padding_calc(img -> width);

    for (uint16_t y = 0; y < img->height; y++) {
        bytesRead = fread(&(img->data[y * img->width]), sizeof(struct pixel), img->width, in);
        if (bytesRead != img->width) {
            free(img->data);
            return READ_INVALID_BITS;
        }
        if (fseek(in, paddingBytes , SEEK_CUR)>=4){
            free(img->data);
            return READ_INVALID_BITS;
        }

    }

    return READ_OK;
}

enum write_status to_bmp(FILE* out, struct image* img){
    BmpHeader new = {
        .bfType = BfType,
        .bfileSize = (img->width * img->height * sizeof(struct pixel)+ img->height* ( (4 - ((sizeof(struct pixel) * img->width) % 4)) )) * sizeof(struct bmp_header),
        .bfReserved = Zero_value,
        .bOffBits = sizeof(struct bmp_header),
        .biSize = Bi_size,
        .biWidth = img->width,
        .biHeight = img->height,
        .biPlanes = Bi_planes,
        .biBitCount = Bit_count,
        .biCompression = Zero_value,
        .biSizeImage = img->width * img->height * sizeof(struct pixel)+ ( (4 - ((sizeof(struct pixel) * img->width) % 4)) ) * img->height,
        .biXPelsPerMeter = Zero_value,
        .biYPelsPerMeter = Zero_value,
        .biClrUsed = Zero_value,
        .biClrImportant = Zero_value
    };

    if (out == NULL) {
        return WRITE_ERROR;
    }

    if (fwrite(&new, sizeof(BmpHeader), 1, out) != 1) {
        return WRITE_ERROR;
    }

    uint8_t paddingBytes = padding_calc(img -> width);

    for (uint16_t y = 0; y < img->height; y++) {

        if (fwrite(&(img->data[y * img->width]), sizeof(struct pixel), img->width, out) != img->width) {
            return WRITE_ERROR;
        }

        for (uint32_t i = 0; i < paddingBytes; i++) {
            uint8_t padding = 0;
            if ( fwrite(&padding, sizeof(uint8_t), 1, out)!= 1) {
                return WRITE_ERROR;
            }

        }
    }

    free(img->data);
    return WRITE_OK;
}
