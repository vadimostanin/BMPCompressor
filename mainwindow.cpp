#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "directorycontent.h"
#include "helpers.h"
#include "imageprocessingrequest.h"
#include "imageprocessingstartresponse.h"
#include "imageprocessingresultresponse.h"
#include <filesystem>
#include <iostream>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    const uint filesCount = DirectoryContent::Instance().getFiles().size();
    const auto fileInfos = DirectoryContent::Instance().getFiles();
    auto table = ui->filesTableWidget;

    for(uint i = 0 ; i < filesCount ; ++i)
    {
        const auto extension = std::filesystem::path(fileInfos[i].name).extension();
        std::cout << "extension=" << extension << std::endl;
        if(extension != ".bmp" && extension != ".png" && extension != ".barch")
        {
            continue;
        }
        table->setRowCount(table->rowCount() + 1);
        QTableWidgetItem *item = nullptr;
        item = new QTableWidgetItem(QString(fileInfos[i].name.c_str()));
        item->textAlignment();
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(i, 0, item);

        item = new QTableWidgetItem(QString(HumanReadableSize(fileInfos[i].size).c_str()));
        item->textAlignment();
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(i, 1, item);

        item = new QTableWidgetItem();
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(i, 2, item);
    }
    table->resizeColumnsToContents();
    table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    connect(ui->filesTableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(onCellDoubleClicked(int,int)));
    connect(ui->filesTableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onCellChanged()));
    qRegisterMetaType<IResponse>("IResponse");
    qRegisterMetaType<std::shared_ptr<IResponse>>("std::shared_ptr<IResponse>");
    connect(&mImageProcMgr, SIGNAL(started(std::shared_ptr<IResponse>)), this, SLOT(onImageProcStarted(std::shared_ptr<IResponse>)));
    connect(&mImageProcMgr, SIGNAL(result(std::shared_ptr<IResponse>)), this, SLOT(onImageProcResult(std::shared_ptr<IResponse>)));

    QHBoxLayout *layout = new QHBoxLayout(this->centralWidget());
    layout->addWidget(ui->filesTableWidget);
    layout->addWidget(ui->imageLabel);

    ui->imageLabel->show();

    ui->statusbar->showMessage("One-Click - show image;   Double-click - encode/decode");
}

void MainWindow::onCellChanged()
{
    auto table = ui->filesTableWidget;
    if(table->selectedItems().empty())
    {
        return;
    }
    const uint row = table->selectedItems()[0]->row();
    const auto name = table->item(row, 0)->text();
    auto extention = std::filesystem::path(name.toStdString()).extension();
    if(extention != ".bmp" && extention != ".png")
    {
        ui->imageLabel->clear();
        return;
    }

    const auto path = QString((DirectoryContent::Instance().getDirectory() + "/").c_str()) + name;
    QImage image(path);
    const auto height = ui->centralwidget->height() - 50;
    ui->imageLabel->setPixmap(QPixmap::fromImage(image).scaledToHeight(height));
    ui->imageLabel->show();
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    auto pixmap = ui->imageLabel->pixmap();
    if(nullptr == pixmap)
    {
        return;
    }
    // auto height = ui->centralwidget->height();
    // std::cout << "height=" << height << std::endl;
    // pixmap->scaledToHeight(height/2);
    // ui->imageLabel->setPixmap(*pixmap);
    // ui->imageLabel->update();
    // ui->imageLabel->updateGeometry();
    // ui->imageLabel->show();
    // const auto height = ui->centralwidget->height();
    // ui->imageLabel->setPixmap(QPixmap::fromImage(ui->imageLabel->pixmap()->toImage()).scaledToHeight(height));
    // ui->imageLabel->show();
}

void MainWindow::onCellDoubleClicked(int row, int column)
{
    auto table = ui->filesTableWidget;
    auto path = (DirectoryContent::Instance().getDirectory() + "/") + table->item(row, 0)->text().toStdString();
    auto request = std::make_shared<ImageProcessingRequest>(path, row);
    mImageProcMgr.addRequest(request);
}

void MainWindow::onImageProcStarted(std::shared_ptr<IResponse> iresponse)
{
    const auto response = std::dynamic_pointer_cast<ImageProcessingStartResponse>(iresponse);
    auto table = ui->filesTableWidget;
    const uint row = response->row();
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    for(uint i = 0 ; i < table->columnCount() ; ++i)
    {
        table->item(row, i)->setBackgroundColor(QColor(255, 255, 0));
    }
    table->item(row, 2)->setText(QString(response->message().c_str()));
    table->resizeColumnsToContents();
    std::cout << "path=" << response->message() << ", row=" << response->row() << std::endl;
}

int MainWindow::findTableRowWithName(QString name)
{
    auto table = ui->filesTableWidget;
    for(int row_i = 0 ; row_i < table->rowCount() ; ++row_i)
    {
        if(table->item(row_i, 0)->text() == name)
        {
            return row_i;
        }
    }
    return -1;
}

void MainWindow::onImageProcResult(std::shared_ptr<IResponse> iresponse)
{
    const auto response = std::dynamic_pointer_cast<ImageProcessingResultResponse>(iresponse);
    auto table = ui->filesTableWidget;
    const uint row = response->row();
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    for(uint i = 0 ; i < table->columnCount() ; ++i)
    {
        table->item(row, i)->setBackgroundColor(QColor(255, 255, 255));
    }
    table->item(row, 2)->setText("");
    std::cout << "path=" << response->path() << ", row=" << response->row() << std::endl;

    const auto name_new = QString(std::filesystem::path(response->path()).filename().c_str());
    const int rowWithSameName = findTableRowWithName(name_new);
    if(rowWithSameName == -1)
    {
        table->setRowCount(table->rowCount() + 1);
        const uint new_row = table->rowCount() - 1;
        QTableWidgetItem *item = nullptr;
        item = new QTableWidgetItem(name_new);
        item->textAlignment();
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(new_row, 0, item);

        item = new QTableWidgetItem(QString(HumanReadableSize(response->filesize()).c_str()));
        item->textAlignment();
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(new_row, 1, item);

        item = new QTableWidgetItem();
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(new_row, 2, item);
    }
    else
    {
        table->item(rowWithSameName, 1)->setText(QString(HumanReadableSize(response->filesize()).c_str()));
    }

    table->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}
