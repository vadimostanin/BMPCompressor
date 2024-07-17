#ifndef _TYPES_INTERNAL_H_
#define _TYPES_INTERNAL_H_

struct RawImageData {
    int width; // image width in pixels
    int height; // image height in pixels
    unsigned char* data; // Pointer to image data. data[j * width + i] is color of pixel in row j and column i.
};

#endif
