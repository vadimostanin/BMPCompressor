#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageprocessingmgr.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onCellDoubleClicked(int row, int column);
    void onCellChanged();
    void onImageProcStarted(std::shared_ptr<IResponse>);
    void onImageProcResult(std::shared_ptr<IResponse>);
private:
    int findTableRowWithName(QString name);
    void resizeEvent(QResizeEvent*);
private:
    Ui::MainWindow *ui;
    ImageProcessingMgr mImageProcMgr;
};
#endif // MAINWINDOW_H
