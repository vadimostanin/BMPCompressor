#include "imageprocessingmgr.h"
#include "imageprocessing.h"
#include "imageprocessingrequest.h"
#include <iostream>
#include <QThread>
#include <memory>

ImageProcessingMgr::ImageProcessingMgr()
{
}

void ImageProcessingMgr::addRequest(IShRequest irequest)
{
    auto request = std::dynamic_pointer_cast<ImageProcessingRequest>(irequest);
    if(nullptr == request)
    {
        std::cerr << "Can't convert IShRequest to ImageProcessingRequest" << std::endl;
        return;
    }
    auto processing = new ImageProcessing(request);//std::make_shared<ImageProcessing>(request);
    QThread* thread = new QThread;
    connect(thread, SIGNAL(started()), processing, SLOT(process()));
    connect(processing, SIGNAL(finished()), thread, SLOT(quit()));
    connect(processing, SIGNAL(started(std::shared_ptr<IResponse>)), this, SLOT(onImgprocStarted(std::shared_ptr<IResponse>)));
    connect(processing, SIGNAL(result(std::shared_ptr<IResponse>)), this, SLOT(onImgprocResult(std::shared_ptr<IResponse>)));

    processing->moveToThread(thread);

    thread->start();
}

void ImageProcessingMgr::onImgprocStarted(std::shared_ptr<IResponse> response)//ImageProcessingResponse
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    // std::cout << "path=" << response.path() << ", row=" << response.row() << std::endl;
    emit started(response);
}

void ImageProcessingMgr::onImgprocResult(std::shared_ptr<IResponse> response)//ImageProcessingResponse
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    // std::cout << "path=" << response.path() << ", row=" << response.row() << std::endl;
    emit result(response);
}
