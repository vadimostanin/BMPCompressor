#include "imageprocessingrequest.h"

ImageProcessingRequest::ImageProcessingRequest(std::string path, uint row):
    mPath(std::move(path)), mRow(row)
{}
