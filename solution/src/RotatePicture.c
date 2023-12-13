#include "RotatePicture.h"
#include <stdlib.h> 

struct image rotate(struct image source){
    struct image newImg = {
        .width = source.height,
        .height = source.width,
        .data = (struct pixel*)malloc(sizeof(struct pixel) * source.width * source.height)
    };

    if (newImg.data == NULL) {
        newImg.width = 0;
        newImg.height = 0;
        return newImg;
    }

    for (uint16_t y = 0; y < source.height; y++) {
        for (uint16_t x = 0; x < source.width; x++) {
            newImg.data[x * newImg.width + newImg.width - y - 1] = source.data[y * source.width + x];
        }
    }

    return newImg;
}
