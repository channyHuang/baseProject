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

    m_thread = new QThread();
    m_analyseLabels = new AnalyseLabels();
    m_analyseLabels->moveToThread(m_thread);
    m_thread->start();
    connect(this, &MainWidget::sigHasData, m_analyseLabels, &AnalyseLabels::analyse);

    std::cout << "start to read file" << std::endl;
    if(file.open(QIODevice::ReadOnly)) {
        std::cout << "open file" << std::endl;
        char buffer[4096];
        qint64 lineLen = file.readLine(buffer , sizeof(buffer));
        while (lineLen != -1)
        {
            QString qsBuffer = QString(buffer);
            //labels.analyse(qsBuffer);
            QTimer::singleShot(0, this, [this, qsBuffer]{
                emit sigHasData(qsBuffer);
            });
            lineLen = file.readLine(buffer , sizeof(buffer));
        }
    }
    file.close();
    std::cout << "finish read file" << std::endl;
    std::cout << m_analyseLabels->count << std::endl;
}
