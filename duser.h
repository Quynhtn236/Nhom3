#ifndef DUSER_H
#define DUSER_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "databasemanager.h"

namespace Ui {
class duser;
}

class duser : public QDialog
{
    Q_OBJECT

public:
    explicit duser(QWidget *parent = nullptr);
    ~duser();

private:
    Ui::duser *ui;
    QSqlQuery query;
    DatabaseManager m_db;
};

#endif // DUSER_H
