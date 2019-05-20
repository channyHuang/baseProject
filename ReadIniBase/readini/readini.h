#ifndef READINI_H
#define READINI_H

#include <QObject>
#include <QSettings>
#include <QMap>
#include <QDir>

class ReadIni : public QObject
{
public:
    explicit ReadIni(QObject *parent = 0);
    virtual ~ReadIni();

    bool setIniName(QString sIniName);

    QString getValue(QString sSection, QString sKey, QString sDefault = "");
    bool setValue(QString sSection, QString sKey, QString sValue);

private:
    bool readIniFile();

private:
    QString m_sRootSectionName;

    QString m_sIniName;
    QMap<QString, QMap<QString, QString>> m_qmKeyValues;
};

#endif // READINI_H
