#include "mainwindow.h"
#include "helpers.h"
#include "directorycontent.h"
#include <QApplication>
#include <iostream>
#include "dialogerror.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::string directory;
    std::vector<std::string> extentions;
    std::tie(directory, extentions) = extractArgs(argc, argv);
    try
    {
        if(extentions.empty())
        {
            extentions = {"*.bmp", "*.barch", "*.png"};
        }
        DirectoryContent::Instance().applyDirectory(directory, extentions);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what();
        DialogError dlgError(e.what());
        // dlgError.show();
        return a.exec();
    }

    // return 0;

    MainWindow w;
    w.show();
    return a.exec();
}
