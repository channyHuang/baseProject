#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "ahttprqtuploadimage.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    btnSendImage = new QPushButton(this);
    btnSendImage->setText("Send Image");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(btnSendImage);
    setLayout(mainLayout);

    connect(btnSendImage, &QPushButton::clicked, this, &MainWidget::slt_sendImage);

    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::slt_sendImage()
{
    QVariantMap rspResult;
    T_UploadImage stUploadImage;

    memcpy(stUploadImage.imageName, "testimg.jpeg", 20);
    memcpy(stUploadImage.imageParamName, "image", 20);
    memcpy(stUploadImage.binImageName, "testimg.jpeg", 20);
    memcpy(stUploadImage.binImageParamName, "binImage", 20);

    AHttpRqtUploadImage rqt("http://127.0.0.1:7999");
    rqt.doHttpRqt(&stUploadImage, rspResult);
}
