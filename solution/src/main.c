#include "BMPConverter.h"
#include "RotatePicture.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE* file1 = fopen(argv[1], "rb");
    FILE* file2 = fopen(argv[2], "wb");

    if (file1 == NULL || file2 == NULL) {
        printf("Error opening files.\n");
        return 1;
    }

    struct image img = {.width = 0, .height = 0, .data = NULL};
    enum read_status statusFromBMP = from_bmp(file1, &img);
    
    if (statusFromBMP == READ_OK) {
        struct image imgRotated = rotate(img);
        
        if (to_bmp(file2, &imgRotated) == WRITE_OK) {
            printf("Success\n");
        } else {
            printf("Write error\n");
        }
        
        free(imgRotated.data);
    } else if (statusFromBMP == READ_INVALID_SIGNATURE) {
        printf("Invalid signature\n");
    } else if (statusFromBMP == READ_INVALID_BITS) {
        printf("Invalid bits\n");
    } else if (statusFromBMP == READ_INVALID_HEADER) {
        printf("Invalid header\n");
    }

    fclose(file1);
    fclose(file2);

    return 0;
}
