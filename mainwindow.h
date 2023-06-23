#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void IC_test();

private slots:

    void on_btnHis_clicked();

    void on_btnUser_clicked();

    void on_btnCards_clicked();

private:
    Ui::MainWindow *ui;
    QSqlQuery query;
    DatabaseManager m_db;
    bool cDB;
};
#endif // MAINWINDOW_H
