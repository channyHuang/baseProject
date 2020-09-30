#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>

#include "encodesdk.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void onFileSelected();

private:
    Ui::Widget *ui;

    bool init();
    void uninit();
    EncodeSDK sdk;
    bool bLoadSuccess;

    QPushButton* pSelectBtn;
};

#endif // WIDGET_H
