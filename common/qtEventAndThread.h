#ifndef QTEVENTANDTHREAD_H
#define QTEVENTANDTHREAD_H

#include <QCoreApplication>
#include <QThread>

class QtEvent
{
public:
    QtEvent() {
        if(NULL == QCoreApplication::instance())
        {
            int argc=1;
            QString name = "demoDll";
            char *argv = (char*)name.toStdString().c_str();
            m_pApp = new QCoreApplication(argc,&argv);
        }
    }

    virtual ~QtEvent() {
        if(m_pApp)
            delete m_pApp;
        m_pApp = nullptr;
    }

private:
    QCoreApplication *m_pApp;
};

template<typename T>
class DllThread : public QThread
{
public:
    explicit DllThread(QObject *parent = Q_NULLPTR)
        :QThread(parent),m_pt(nullptr){}
    virtual ~DllThread(){}

    void bind(T* pt)    {
        m_pt = pt;
        m_pt->moveToThread(this);
    }
protected:
    virtual void run()    {
        if(nullptr == m_pt) return;
        m_pt->initInThread();
        exec();
        m_pt->deinitInThread();
    }
public:
    T* m_pt;
};

#endif // QTEVENTANDTHREAD_H
