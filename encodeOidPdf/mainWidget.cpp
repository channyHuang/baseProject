#include "mainWidget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QVBoxLayout* pRootLayout = new QVBoxLayout(this);

    QTextEdit* pFileLine = new QTextEdit(this);
    pFileLine->setPlaceholderText(tr("Select PDF file"));
    pSelectBtn = new QPushButton(this);
    pSelectBtn->setText(tr("Choose from device"));
    QHBoxLayout* pFileSelectLayout = new QHBoxLayout;
    pFileSelectLayout->addWidget(pFileLine);
    pFileSelectLayout->addWidget(pSelectBtn);

    QGridLayout* pGridLayout = new QGridLayout;

    pRootLayout->addLayout(pFileSelectLayout);
    pRootLayout->addLayout(pGridLayout);

    bLoadSuccess = init();
    if (bLoadSuccess) {
        int mainVer, subVer;
        sdk.DLL_dllVersion(&mainVer, &subVer);
        qDebug() << mainVer << " " << subVer;

        //Step 1. 设定码点尺寸
        long dpi=600;//码点解析度设为600dpi; 目前只能设600或1200
        long dotSzX=2,dotSzY=2;//码点大小 DotSize=2x2; 数值1至4
        sdk.DLL_setPatternFormat(dpi,dotSzX,dotSzY,TRUE);

        //Step 2. 设定进度条Callback 函数，请参看progressTest源码
        //sdk.setRunProgress(progressTest,(DWORD)&m_progress);
    }

        {//Step 4. 画码区块
            long id=10001;
            long left=219;
            long top=128;
            long right=819;
            long bottom=189;
            BOOL bFrame=TRUE;
            BOOL bNumber=TRUE;

            //画方
            sdk.DLL_ydDrawRect(hPdf,id,left,top,right,bottom,bFrame,bNumber);

            //画椭圆
            sdk.DLL_ydDrawEllipse(hPdf,id,123,289,187,357,bFrame,bNumber);

            //画多边形
            long poly[]={208,469, 850,469, 850,532, 573,532, 573,598, 208,598};
            sdk.DLL_ydDrawPoly(hPdf,id,poly,6,bFrame,bNumber);
        }
        //------------------------
        //Step 5. 输出码图
        //sdk.DLL_writeToTif(hPdf,"output.tif");

        //Step 5. 输出PDF=码图+底图
        sdk.DLL_writeToPdf(hPdf,"output.pdf");

        //Step 6.释放记忆体
        sdk.DLL_pdfRelease(hPdf);

}

Widget::~Widget()
{
    uninit();
    delete ui;
}

bool Widget::init()
{
    return sdk.load("./yDotPdfDll.dll");
}

void Widget::uninit()
{
    sdk.unload();
}

void Widget::onFileSelected()
{
    //Step 3. 读入底图example.jpg；背景图的解析读最好为300dpi
    DWORD hPdf=sdk.DLL_pdfCreate("example.png");
}
