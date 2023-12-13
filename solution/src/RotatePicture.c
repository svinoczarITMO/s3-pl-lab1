#include "RotatePicture.h"
#include <stdlib.h> 

struct image rotate(struct image source){
    if (source.data != NULL){
        struct image NewImg = {
            .width = source.height,
            .height = source.width
        };
        NewImg.data = (struct pixel*)malloc(sizeof(struct pixel) * NewImg.width * NewImg.height);

        for (uint16_t y = 0; y < source.height; y++){
            for (uint16_t x = 0; x < source.width; x++){
                NewImg.data[x * NewImg.width + source.height - y - 1] = source.data[y * source.width + x];
            }
        }

        free(source.data);
        return NewImg;
    }
    else{
        struct image nullImg = {
            .width = 0,
            .height = 0,
            .data = NULL
        };
        return nullImg;
    }
}