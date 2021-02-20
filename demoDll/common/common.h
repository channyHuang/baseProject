#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QCryptographicHash>

class CommonFunc {
public:
    CommonFunc();
    static void writeLog(const QString &sLog);
    static QString encodePassword(const QString& qsPassword);
};

#endif // COMMON_H
