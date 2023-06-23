#ifndef HISTORY_H
#define HISTORY_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class history;
}

class history : public QDialog
{
    Q_OBJECT

public:
    explicit history(QWidget *parent = nullptr);
    ~history();

private:
    Ui::history *ui;
    QSqlQuery query;
};

#endif // HISTORY_H
