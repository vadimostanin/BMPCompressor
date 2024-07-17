#ifndef BMPCOMPRESSOR_H
#define BMPCOMPRESSOR_H

#include "types_public.h"

std::vector<unsigned char> encodeImage(const RawImageData& image);
RawImageData decodeImage(const std::vector<unsigned char>& encodedData);

#endif // BMPCOMPRESSOR_H
