#include "BMPConverter.h"
#include "RotatePicture.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum write_status to_bmp(FILE* out, struct image* img){

    char padding = 0;

    struct bmp_header new = {
            .biWidth = img -> width,
            .biHeight = img -> height,
            .bfileSize = img-> width * img -> height * sizeof(pixel)+54,
            .biSizeImage = new.biSizeImage = img-> width * img -> height * sizeof(pixel),
            .bfType = 0x4d42,
            .bfReserved = 0,
            .bOffBits = 54,
            .biSize = 40,
            .biPlanes = 1,
            .biBitCount = 24,
            .biCompression = 0,
            .biXPelsPerMeter = 0,
            .biYPelsPerMeter = 0,
            .biClrUsed = 0,
            .biClrImportant = 0
    };

    if (out == NULL) return WRITE_ERROR;
    fwrite(&new, sizeof(struct bmp_header), 1, out);
    for (uint16_t y = 0; y < img->height; y++){
        fwrite(&(img -> data[y * img -> width]), sizeof(struct pixel), img -> width, out);
        for (uint32_t i = 0; i < (uint32_t) ((4 - ((img->width * sizeof(struct pixel)) % 4)) % 4); i++){
            fwrite(&padding, sizeof(uint8_t), 1, out);
        }
    }
    free(img -> data);
    return WRITE_OK;
}

enum read_status from_bmp(FILE* in, struct image* img){
    if (in != NULL){
        struct bmp_header old;
        size_t bytesRead = fread(&old, sizeof(old), 1, in);
        if (bytesRead == 1){
            img->width = old.biWidth;
            img->height = old.biHeight;

            img->data = (struct pixel*)malloc(sizeof(struct pixel) * img->width * img->height);
            if (img->data != NULL) {
                fseek(in, old.bOffBits, SEEK_SET);
                for (uint16_t y = 0; y < img->height; y++){
                    fread(&(img -> data[y * img -> width]),sizeof(struct pixel),img -> width,in);
                    fseek(in, (uint32_t) ((4 - ((img->width * sizeof(struct pixel)) % 4)) % 4), SEEK_CUR);
                }
                return READ_OK;
            }

            return READ_INVALID_BITS;

        }
        return READ_INVALID_HEADER;
    }
    return READ_INVALID_SIGNATURE;

}
