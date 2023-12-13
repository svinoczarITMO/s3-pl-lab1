#include "HeaderStructure.h"
#include "ImagePixelStructure.h"
#include "BMPConverter.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum write_status to_bmp(FILE* out, struct image* img) {
    struct bmp_header new = {
        .bfType = 0x4d42,
        .bfileSize = img->width * img->height * sizeof(struct pixel) + 54,
        .bfReserved = 0,
        .bOffBits = 54,
        .biSize = 40,
        .biWidth = img->width,
        .biHeight = img->height,
        .biPlanes = 1,
        .biBitCount = 24,
        .biCompression = 0,
        .biSizeImage = img->width * img->height * sizeof(struct pixel),
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

enum read_status from_bmp(FILE* in, struct image* img) {
    struct bmp_header old;
    size_t bytesRead = fread(&old, sizeof(struct bmp_header), 1, in);
    if (bytesRead != 1) {
        return READ_INVALID_HEADER;
    }

    if (old.bfType != 0x4d42) {
        return READ_INVALID_SIGNATURE;
    }

    img->width = old.biWidth;
    img->height = old.biHeight;

    img->data = (struct pixel*)malloc(sizeof(struct pixel) * img->width * img->height);
    if (img->data == NULL) {
        return READ_INVALID_BITS;
    }

    fseek(in, old.bOffBits, SEEK_SET);

    for (uint16_t y = 0; y < img->height; y++) {
        fread(&(img->data[y * img->width]), sizeof(struct pixel), img->width, in);

        uint32_t paddingBytes = (4 - ((img->width * sizeof(struct pixel)) % 4)) % 4;
        fseek(in, paddingBytes, SEEK_CUR);
    }

    return READ_OK;
}
