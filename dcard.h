#ifndef DCARD_H
#define DCARD_H

#include <QDialog>
#include <QTableView>
#include <QHeaderView>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "databasemanager.h"

static QString idcard;

namespace Ui {
class dcard;
}

class dcard : public QDialog
{
    Q_OBJECT

public:
    explicit dcard(QWidget *parent = nullptr);
    ~dcard();
private slots:
    void resetTable();
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void on_pushButton_clicked();

    void on_btnAdd_clicked();

    void on_btnDel_clicked();

private:
    Ui::dcard *ui;

    QSqlQuery query;
    DatabaseManager m_db;
};

#endif // DCARD_H
