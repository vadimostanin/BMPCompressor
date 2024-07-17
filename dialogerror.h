#ifndef DIALOGERROR_H
#define DIALOGERROR_H

#include <QtGui>
#include <QDialog>
#include <QPushButton>
#include <QObject>
#include <QMainWindow>
#include <QLabel>
#include <QBoxLayout>

class DialogError : public QMainWindow
{
    Q_OBJECT;
public:
    DialogError(std::string error)
    {
        mDialog = new QDialog(this);
        mDialog->setWindowTitle("Dialog");
        mDialog->setSizeGripEnabled(false);
        QLabel *label = new QLabel(mDialog);
        label->setText(QString(error.c_str()));

        mButton = new QPushButton("Ok", mDialog);
        QBoxLayout *layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, mDialog);
        mDialog->setLayout(layout);
        layout->addWidget(label);
        layout->addWidget(mButton);
        connect(mButton, SIGNAL(clicked()), this, SLOT(on_exit()));
        connect(mButton, SIGNAL(pressed()), this, SLOT(on_pressed()));
        connect(mButton, SIGNAL(released()), this, SLOT(on_released()));
        mDialog->show();
    }
public slots:
    void on_exit()
    {
        mDialog->close();
    }
    void on_pressed()
    {
        mStyleSheet = mButton->styleSheet();
        mButton->setStyleSheet("color: black; background-color: lightgreen; border-width: 6px; border-style: solid; border-color: lightgreen;");
        mButton->update();
    }
    void on_released()
    {
        mButton->setStyleSheet(mStyleSheet);
        mButton->update();
    }
private:
    QDialog *mDialog{nullptr};
    QPushButton *mButton{nullptr};
    QString mStyleSheet;
};

#endif // DIALOGERROR_H
