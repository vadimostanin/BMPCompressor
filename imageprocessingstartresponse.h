#ifndef IMAGEPROCESSINGSTARTRESPONSE_H
#define IMAGEPROCESSINGSTARTRESPONSE_H

#include "iresponse.h"
#include <string>

class ImageProcessingStartResponse : public IResponse
{
public:
    ImageProcessingStartResponse() = default;
    std::string message() const { return mMessage; }
    void message(std::string message) { mMessage = message; }
    uint row () const { return mRow; }
    void row(uint row) { mRow = row; }
    virtual ~ImageProcessingStartResponse(){}
private:
    std::string mMessage;
    uint mRow{0};
};

#endif // IMAGEPROCESSINGSTARTRESPONSE_H
