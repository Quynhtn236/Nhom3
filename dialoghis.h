#ifndef DIALOGHIS_H
#define DIALOGHIS_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "databasemanager.h"

namespace Ui {
class dialoghis;
}

class dialoghis : public QDialog
{
    Q_OBJECT

public:
    explicit dialoghis(QWidget *parent = nullptr);
    ~dialoghis();

private:
    Ui::dialoghis *ui;
     QSqlQuery query;
     DatabaseManager m_db;
};

#endif // DIALOGHIS_H
