#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    if (QSqlDatabase::contains(m_qsDatabaseName)) {
        m_sqlDb = QSqlDatabase::database(m_qsDatabaseName);
    } else {
        m_sqlDb = QSqlDatabase::addDatabase("QSQLITE");
        m_sqlDb.setDatabaseName(QCoreApplication::applicationDirPath() + "/" + m_qsDatabaseName + ".db");
    }
    init();
}

DatabaseManager::~DatabaseManager()
{

}

bool DatabaseManager::judgeTableExist(const QString& qsTableName) {
    if (!m_sqlDb.open()) {
        return false;
    }
    QString qsCheckTableSql = QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(qsTableName);
    QSqlQuery query(m_sqlDb);
    query.prepare(qsCheckTableSql);
    if (!query.exec()) {
        m_sqlDb.close();
        return false;
    }
    if (query.next()) {
        if (query.value(0).toInt() > 0) {
            m_sqlDb.close();
            return true;
        }
    }
    m_sqlDb.close();
    return false;
}

bool DatabaseManager::execSql(const QString& qsSqlSentence) {
    if (!m_sqlDb.open()) {
        return false;
    }
    QSqlQuery query(m_sqlDb);
    query.prepare(qsSqlSentence);
    if (!query.exec()) {
        //LOG_ERROR("%s-%s(%s) exec [%s] failed, error = %s", __FILE__, __FUNCTION__, __LINE__, qsSqlSentence.toStdString().c_str(), m_sqlDb.lastError().text().toStdString().c_str());
        m_sqlDb.close();
        return false;
    }
    m_sqlDb.close();
    return true;
}

bool DatabaseManager::init() {
    if (!m_sqlDb.open()) {
        return false;
    }

    bool res = judgeTableExist(m_qsUserTableName);
    if (res) return true;

    QString qsCreateSentence = QString().asprintf("create table %s (username varchar(15) primary key, lastlogintime varchar(30))", m_qsUserTableName.toStdString().c_str());
    res = execSql(qsCreateSentence);

    return res;
}

bool DatabaseManager::insertHistoryUser(const QString& qsUserName)
{
    if (!m_sqlDb.open()) {
        return false;
    }
    QSqlQuery query(m_sqlDb);
    QDateTime qdtCurTime = QDateTime::fromString("1970-01-01 00:00:00.000");
    QString qsInsertSentence = QString().asprintf("insert into %s (username, lastlogintime) values ('%s', '%s')",
                                                 m_qsUserTableName.toStdString().c_str(),
                                                 qsUserName.toStdString().c_str(),
                                                 qdtCurTime.toString("yyyy-MM-dd hh:mm:ss.zzz").toStdString().c_str());
    query.prepare(qsInsertSentence);
    if (!query.exec()) {
        m_sqlDb.close();
        return false;
    }
    m_sqlDb.close();
    return true;
}

bool DatabaseManager::updateHistoryUser(const QString& qsUserName)
{
    if (!m_sqlDb.open()) {
        return false;
    }
    QSqlQuery query(m_sqlDb);
    QDateTime qdtCurTime = QDateTime::currentDateTime();
    QString qsInsertSentence = QString().asprintf("update %s set lastlogintime = '%s' where username = '%s'",
                                                 m_qsUserTableName.toStdString().c_str(),
                                                 qdtCurTime.toString("yyyy-MM-dd hh:mm:ss.zzz").toStdString().c_str(),
                                                 qsUserName.toStdString().c_str());
    query.prepare(qsInsertSentence);
    if (!query.exec()) {
        m_sqlDb.close();
        return false;
    }
    m_sqlDb.close();
    return true;
}

QHash<QString, QDateTime> DatabaseManager::checkUserHistory() {
    QHash<QString, QDateTime> qlHistory;
    if (!m_sqlDb.open()) {
        return qlHistory;
    }

    QString qsCheckHistoryDb = QString().asprintf("select * from %s", m_qsUserTableName.toStdString().c_str());
    QSqlQuery query(m_sqlDb);
    query.prepare(qsCheckHistoryDb);
    if (!query.exec()) {
        m_sqlDb.close();
        return qlHistory;
    }

    while (query.next()) {
        qlHistory[query.value(0).toString()] = query.value(1).toDateTime();
    }

    m_sqlDb.close();
    return qlHistory;
}
