#ifndef MODULE_OF_ROTATE_PICTURE
#define MODULE_OF_ROTATE_PICTURE

struct pixel{
    uint8_t b, g, r;
};

struct image{
    uint16_t width, height;
    struct pixel* data;
};

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

struct image rotate(struct image source);

#endif
