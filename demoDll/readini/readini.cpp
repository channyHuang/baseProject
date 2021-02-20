#include "readini.h"

ReadIni::ReadIni(QObject *parent)
    : QObject(parent)
    , m_sRootSectionName("RootSectionName")
{

}

ReadIni::~ReadIni()
{
    m_qmKeyValues.clear();
}

bool ReadIni::setIniName(QString sIniName)
{
    m_sIniName = sIniName;
    if (false == QFile(m_sIniName).exists()) return false;

    return readIniFile();
}

bool ReadIni::readIniFile()
{
    m_qmKeyValues.clear();

    QSettings qsSettings(m_sIniName, QSettings::IniFormat);
    QStringList qslSections = qsSettings.childGroups();

    {
        QString sSection = "";
        QMap<QString, QString> qmKeyValue;
        qsSettings.beginGroup(sSection);
        QStringList qslKeys = qsSettings.allKeys();
        for (QString sKey : qslKeys)
            qmKeyValue[sKey] = qsSettings.value(sKey, "").toString();
        m_qmKeyValues[m_sRootSectionName] = qmKeyValue;
    }

    for (QString sSection : qslSections)
    {
        QMap<QString, QString> qmKeyValue;

        qsSettings.beginGroup(sSection);
        QStringList qslKeys = qsSettings.allKeys();
        for (QString sKey : qslKeys)
            qmKeyValue[sKey] = qsSettings.value(sKey, "").toString();

        m_qmKeyValues[sSection] = qmKeyValue;
    }
    qsSettings.endGroup();

    return true;
}

QString ReadIni::getValue(QString sSection, QString sKey, QString sDefault)
{
    if (sSection.isEmpty()) sSection = m_sRootSectionName;

    if (0 >= m_qmKeyValues.count(sSection)) return sDefault;

    if (0 >= m_qmKeyValues[sSection].count(sKey)) return sDefault;

    return m_qmKeyValues[sSection][sKey];
}

bool ReadIni::setValue(QString sSection, QString sKey, QString sValue)
{
    QSettings qsSettings(m_sIniName, QSettings::IniFormat);

    if (sSection.isEmpty())
    {
        qsSettings.beginGroup("");
        sSection = m_sRootSectionName;
    }
    else
    {
        qsSettings.beginGroup(sSection);
    }
    qsSettings.setValue(sKey, sValue);
    qsSettings.endGroup();
    qsSettings.sync();

    if (0 >= m_qmKeyValues.count(sSection))
    {
        QMap<QString, QString> qmKeyValue;
        qmKeyValue[sKey] = sValue;
        m_qmKeyValues[sSection] = qmKeyValue;
    }
    else
    {
        m_qmKeyValues[sSection][sKey] = sValue;
    }

    return true;
}
