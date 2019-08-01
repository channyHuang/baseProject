#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QFile>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateTime>
#include <QDir>

#include <iostream>



namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void slt_sendImage();

private:
    Ui::MainWidget *ui;

    QPushButton *btnSendImage;
};

#endif // MAINWIDGET_H
