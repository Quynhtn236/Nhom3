#include "history.h"
#include "ui_history.h"
#include <QTableView>
#include <QHeaderView>
#include <QSql>
#include<QSqlDatabase>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

history::history(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::history)
{
    ui->setupUi(this);



}

history::~history()
{
    delete ui;
}
