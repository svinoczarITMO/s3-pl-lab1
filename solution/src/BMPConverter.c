#include "BMPConverter.h"
#include "HeaderStructure.h"
#include "ImagePixelStructure.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BfType 0x4d42

uint8_t padding_calc(uint16_t w){
    return (uint8_t) (4 - ((w * sizeof(struct pixel)) % 4)) % 4;
}

enum read_status from_bmp(FILE* in, struct image* img)
{
    struct bmp_header old;
    size_t bytesRead = fread(&old, sizeof(struct bmp_header), 1, in);
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

    if (fseek(in, bmp_header_local.bOffBits, SEEK_SET) != 0) {
        free(img->data);
        return READ_INVALID_BITS;
    }


    for (uint16_t y = 0; y < img->height; y++) {
        bytesRead = fread(&(img->data[y * img->width]), sizeof(struct pixel), img->width, in);
        if (bytesRead != img->width) {
            free(img->data);
            return READ_INVALID_BITS;
        }
        if (fseek(in, paddingSize, SEEK_CUR)>=4){
            free(img->data);
            return READ_INVALID_BITS;
        }

    }

    return READ_OK;
}

enum write_status to_bmp(FILE* out, struct image* img){
    struct bmp_header new = {
        .bfType = 0x4d42,
        .bfileSize = (img->width * img->height * sizeof(struct pixel)+ img->height* ( (4 - ((sizeof(struct pixel) * img->width) % 4)) )) * sizeof(struct bmp_header),
        .bfReserved = 0,
        .bOffBits = sizeof(struct bmp_header),
        .biSize = 40,
        .biWidth = img->width,
        .biHeight = img->height,
        .biPlanes = 1,
        .biBitCount = 24,
        .biCompression = 0,
        .biSizeImage = img->width * img->height * sizeof(struct pixel)+ ( (4 - ((sizeof(struct pixel) * img->width) % 4)) ) * img->height,
        .biXPelsPerMeter = 0,
        .biYPelsPerMeter = 0,
        .biClrUsed = 0,
        .biClrImportant = 0
    };

    if (out == NULL) {
        return WRITE_ERROR;
    }

    fwrite(&new, sizeof(struct bmp_header), 1, out);

    for (uint16_t y = 0; y < img->height; y++) {
        fwrite(&(img->data[y * img->width]), sizeof(struct pixel), img->width, out);

        uint32_t paddingBytes = (4 - ((img->width * sizeof(struct pixel)) % 4)) % 4;
        for (uint32_t i = 0; i < paddingBytes; i++) {
            uint8_t padding = 0;
            fwrite(&padding, sizeof(uint8_t), 1, out);
        }
    }

    free(img->data);
    return WRITE_OK;
}
