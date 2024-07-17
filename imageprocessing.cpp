#include "imageprocessing.h"
#include "imageprocessingresultresponse.h"
#include "imageprocessingstartresponse.h"
#include "bmpcompressor.h"
#include <iostream>
#include <QImage>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>

ImageProcessing::ImageProcessing(ShImageProcReq request):
    mRequest(request)
{}

void ImageProcessing::saveEncodedImage(const std::vector<unsigned char>& encodedData, const std::string& filename)
{
    std::ofstream outFile(filename, std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(encodedData.data()), encodedData.size());
    outFile.close();
}
#include <string.h>
std::string ImageProcessing::encodeAndSaveImage(std::string path)
{
    QImage image(QString(mRequest->path().c_str()));
    QImage imageConv = image.convertToFormat(QImage::Format::Format_RGB32);
    const uint width = imageConv.width();
    const uint height = imageConv.height();
    std::vector<uint8_t> pixels;
    pixels.resize(width * height);
    uint8_t* pixelsData = pixels.data();
    for(int i = 0 ; i < height ; ++i)
    {
        for(int j = 0 ; j < width ; ++j)
        {
            QRgb rgb = imageConv.pixel(j,i);
            pixelsData[i * width + j] = rgb & 255;
        }
    }

    RawImageData rawData = {(int)width, (int)height, const_cast<unsigned char*>(pixelsData)};
    auto encodedData = encodeImage(rawData);
    std::string path_new = path + "_packed.barch";
    saveEncodedImage(encodedData, path_new);
    return path_new;
}

void ImageProcessing::saveDecodedImage(RawImageData decodedImage, const std::string& filename)
{
    QImage image(decodedImage.width, decodedImage.height, QImage::Format::Format_RGB32);
    for(int i = 0 ; i < decodedImage.height ; ++i)
    {
        for(int j = 0 ; j < decodedImage.width ; ++j)
        {
            const uint8_t gray = decodedImage.data[i * decodedImage.width + j];
            const uint rgb = (gray << 16) | (gray << 8) | (gray << 0);
            image.setPixel(j, i, rgb);
        }
    }
    image.convertToFormat(QImage::Format::Format_Indexed8).save(QString(filename.c_str()));
}

std::string ImageProcessing::decodeAndSaveBMPImage(std::string path)
{
    std::ifstream inFile(path, std::ios::binary | std::ios::ate);
    std::streamsize size = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<unsigned char> loadedEncodedData(size);
    inFile.read(reinterpret_cast<char*>(loadedEncodedData.data()), size);
    inFile.close();
    RawImageData decodedImage = decodeImage(loadedEncodedData);
    std::string path_new = path + "_unpacked.bmp";
    saveDecodedImage(decodedImage, path_new);
    free(decodedImage.data);
    return path_new;
}

void ImageProcessing::process()
{
	std::cout << __func__ << ":" << __LINE__ << std::endl;

    const auto extension = std::filesystem::path(mRequest->path()).extension();
    std::cout << "extension=" << extension << std::endl;
    auto startResponse = std::make_shared<ImageProcessingStartResponse>();
    startResponse->row(mRequest->row());

    auto resultResponse = std::make_shared<ImageProcessingResultResponse>();
    resultResponse->row(mRequest->row());

    if(extension == ".bmp" || extension == ".png")
    {
        startResponse->message("Кодується");
        emit started(startResponse);
        const auto path_new = encodeAndSaveImage(mRequest->path());
        resultResponse->path(path_new);
        resultResponse->filesize(std::filesystem::file_size(std::filesystem::path(path_new)));
    }
    else if(extension == ".barch")
    {
        startResponse->message("Розкодовується");
        emit started(startResponse);
        const auto path_new = decodeAndSaveBMPImage(mRequest->path());
        resultResponse->path(path_new);
        resultResponse->filesize(std::filesystem::file_size(std::filesystem::path(path_new)));
    }
    sleep(3);

    emit result(resultResponse);
    emit finished();
}
