#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSql>
#include<QSqlDatabase>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

class DatabaseManager
{
public:
    DatabaseManager();
    bool openConnection();
    void closeConnection();
    QSqlDatabase& getDatabase();

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
