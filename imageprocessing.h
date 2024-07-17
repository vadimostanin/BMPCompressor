#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "imageprocessingrequest.h"
#include "iresponse.h"
#include "types_internal.h"
#include <QObject>
#include <memory>

class ImageProcessing : public QObject
{
    Q_OBJECT
public:
    ImageProcessing(ShImageProcReq request);
public slots:
    void process();
signals:
    void finished();
    void started(std::shared_ptr<IResponse>);
    void result(std::shared_ptr<IResponse>);
private:
    void saveEncodedImage(const std::vector<unsigned char>& encodedData, const std::string& filename);
    std::string encodeAndSaveImage(std::string path);
    void saveDecodedImage(RawImageData decodedImage, const std::string& filename);
    std::string decodeAndSaveBMPImage(std::string path);
private:
    ShImageProcReq mRequest;
};

typedef std::shared_ptr<ImageProcessing> ShImageProcessing;

#endif // IMAGEPROCESSING_H
