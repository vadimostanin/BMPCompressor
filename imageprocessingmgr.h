#ifndef IMAGEPROCESSINGMGR_H
#define IMAGEPROCESSINGMGR_H

#include <QObject>
#include <queue>
#include "imageprocessingrequest.h"
#include "imageprocessingresultresponse.h"

class ImageProcessingMgr : public QObject
{
    Q_OBJECT
public:
    ImageProcessingMgr();
    void addRequest(IShRequest request);
signals:
    void started(std::shared_ptr<IResponse>);
    void result(std::shared_ptr<IResponse>);
private slots:
    void onImgprocStarted(std::shared_ptr<IResponse>);
    void onImgprocResult(std::shared_ptr<IResponse>);
private:
    std::queue<IShRequest> mRequests;
};

#endif // IMAGEPROCESSINGMGR_H
