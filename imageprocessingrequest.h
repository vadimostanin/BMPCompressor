#ifndef IMAGEPROCESSINGREQUEST_H
#define IMAGEPROCESSINGREQUEST_H

#include "IRequest.h"

class ImageProcessingRequest : public IRequest
{
public:
    ImageProcessingRequest(std::string path, uint row);
    // ~ImageProcessingRequest(){}
    const std::string& path() const{ return mPath; }
    const uint row() const { return mRow; }
private:
    std::string mPath;
    uint mRow{0};
};

typedef std::shared_ptr<ImageProcessingRequest> ShImageProcReq;

#endif // IMAGEPROCESSINGREQUEST_H
