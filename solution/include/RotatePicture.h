#ifndef MODULE_OF_ROTATE_PICTURE
#define MODULE_OF_ROTATE_PICTURE

struct pixel{
    uint8_t b, g, r;
};

struct image{
    uint16_t width, height;
    struct pixel* data;
};

struct image rotate(struct image source);

#endif
