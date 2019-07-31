#include "mainwidget.h"
#include "ui_mainwidget.h"

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

void writeLog(const QByteArray &sLog)
{
    QDateTime dtNow = QDateTime::currentDateTime();

    if (!QDir().exists("Log"))
        QDir().mkdir("Log");

    QString sFileName = "./Log/httpClientTest_"+dtNow.toString("yyyyMMdd")+".log";

    QFile qfLog(sFileName);
    if(qfLog.open(QIODevice::WriteOnly|QIODevice::Append)==false)
    {
        qDebug() << "open log file failed";
        return;
    }

    QByteArray sLogMsg = QByteArray(dtNow.toString("hh:mm:ss.zzz").toStdString().c_str()) + " "+sLog+"\r\n";
    qfLog.write(sLogMsg.data(),sLogMsg.length());

    qfLog.close();
}

void handle_rsp(std::string rsp)
{
    // do sth according to rsp
    std::cout << "http rsp: " << rsp << std::endl;
}

void MainWidget::slt_sendImage()
{
    QString sImageName = "./testimg.jpeg";

    QImage img(sImageName);
    qDebug() << QString::number(QByteArray((char *)img.bits()).length());

    QFile file(sImageName);
    QByteArray sFile;
    if (true == file.open(QIODevice::ReadOnly))
        sFile = file.readAll();
    else
        sFile = "";

    QByteArray sData;
    sData.append(QByteArray::number(sFile.length()));
    sData.append("\r\n\r\n");
    sData.append(sFile);

    std::string url1 = "http://127.0.0.1:7999/api/sendimg";
    HttpClient::SendReq(url1, handle_rsp, sData.data(), sData.length());

    QString sLog = QString().sprintf("Send image %s success, send size = %d", sImageName.toStdString().c_str(), sData.length());
    writeLog(QByteArray(sLog.toStdString().c_str()));
    writeLog(sData.toHex());
}
