#include <QString>
#include <QtTest>

#include "readini.h"

class ReadIniUnitTestTest : public QObject
{
    Q_OBJECT

public:
    ReadIniUnitTestTest();

private Q_SLOTS:
    void testCase1();

    void testCase();
};

ReadIniUnitTestTest::ReadIniUnitTestTest()
{
}

void ReadIniUnitTestTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

void ReadIniUnitTestTest::testCase()
{
    QString sIniName = "./test.ini";

    ReadIni readIni;
    if (QFile(sIniName).exists()) QFile(sIniName).remove();

    //QVERIFY2(false == readIni.setIniName(sIniName), "file not exist");

    QFile qFile(sIniName);
    QVERIFY2(true == qFile.open(QIODevice::ReadWrite), "create file");
    qFile.close();

    QVERIFY2(true == readIni.setIniName(sIniName), "file exist");


    QString sSection = "section", sKey = "key", sValue = "value";
    QVERIFY2("" == readIni.getValue(sSection, sKey), "don't have such key in given section");

    readIni.setValue(sSection, sKey, sValue);
    QVERIFY2(sValue == readIni.getValue(sSection, sKey), "get the right value");

}

QTEST_APPLESS_MAIN(ReadIniUnitTestTest)

#include "tst_readiniunittesttest.moc"
