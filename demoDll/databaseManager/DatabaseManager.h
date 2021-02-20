#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QString>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    static DatabaseManager& getInstance() {
        static DatabaseManager m_instance;
        return m_instance;
    }
    virtual ~DatabaseManager();

    bool init();
    QHash<QString, QDateTime> checkUserHistory();

signals:

private:
    DatabaseManager(QObject *parent = nullptr);

    bool judgeTableExist(const QString& qsTableName);
    bool execSql(const QString& qsSqlSentence);
    bool insertHistoryUser(const QString& qsUserName);
    bool updateHistoryUser(const QString& qsUserName);

    QSqlDatabase m_sqlDb;
    QString m_qsDatabaseName = "HistoryUserDB";
    QString m_qsUserTableName = "userinfo";

};

#endif // DATABASEMANAGER_H
