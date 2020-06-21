#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{

}

void MainWidget::work(QString qsFileName) {
    //std::cout << __FUNCTION__ << " " << QThread::currentThreadId() << std::endl;
    QFile file(qsFileName);
    if (!file.exists()) {
        std::cout << "File " << qsFileName.toStdString().c_str() << " does not exist" << std::endl;
        return;
    }
    AnalyseLabels labels;
    QThread *thread = new QThread();
    labels.moveToThread(thread);
    thread->start();
    connect(this, &MainWidget::sigHasData, &labels, &AnalyseLabels::analyse);

    std::cout << "start to read file" << std::endl;
    if(file.open(QIODevice::ReadOnly)) {
        std::cout << "open file" << std::endl;
        char buffer[4096];
        qint64 lineLen = file.readLine(buffer , sizeof(buffer));
        while (lineLen != -1)
        {
            qsBuffer = QString(buffer);
            //labels.analyse(qsBuffer);
            //QTimer::singleShot(0, this, &MainWidget::sendSig);
            sendSig();

            lineLen = file.readLine(buffer , sizeof(buffer));
        }
    }
    file.close();
    std::cout << "finish read file" << std::endl;
    std::cout << labels.count << std::endl;
}

void MainWidget::sendSig()
{
    emit sigHasData(qsBuffer);
    std::cout << "send sig success" << std::endl;
}
