#ifndef IMAGEPROCESSINGRESULTRESPONSE_H
#define IMAGEPROCESSINGRESULTRESPONSE_H

#include "iresponse.h"
#include <QMetaType>
#include <QString>

class ImageProcessingResultResponse : public IResponse
{
public:
    ImageProcessingResultResponse() = default;
    void path(std::string path) { mPath = path; }
    uint filesize() const { return mFileSize; }
    void row(uint row) { mRow = row; }
    const std::string& path() const{ return mPath; }
    void filesize(uint filesize) { mFileSize = filesize; }
    const uint row() const { return mRow; }
private:
    std::string mPath;
    uint mFileSize{0};
    uint mRow{0};
};

Q_DECLARE_METATYPE(ImageProcessingResultResponse)
typedef std::shared_ptr<ImageProcessingResultResponse> ShImageProcRsp;

#endif // IMAGEPROCESSINGRESULTRESPONSE_H
