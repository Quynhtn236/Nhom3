#ifndef HIS_H
#define HIS_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
     QSqlQuery query;

};

#endif // HIS_H
