#include "BMPConverter.h"
#include "RotatePicture.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

        struct image img =(struct image){ .width=0, .height=0};
        FILE* file1 = (FILE*)fopen(argv[1], "rb");
        FILE* file2 = (FILE*)fopen(argv[2], "wb");
        enum read_status statusFromBMP = from_bmp(file1, &img);
        if (statusFromBMP == READ_OK){
            struct image imgRotated = rotate(img);
            if (to_bmp(file2, &imgRotated) == WRITE_OK){
                printf("Success");
            }
            else{
                printf("Write error");
            }
        }
        if (statusFromBMP== READ_INVALID_SIGNATURE){
            printf("Invalid signature");
        }
        if (statusFromBMP== READ_INVALID_BITS){
            printf("Invalid bits");
        }
        if (statusFromBMP== READ_INVALID_HEADER){
            printf("Invalid header");
        }
        fclose(file1);
        fclose(file2);
    return 0;
}
