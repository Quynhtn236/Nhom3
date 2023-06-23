#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTimer>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "rc522.h"
#include "i2clcd.h"
#include "rwfile.h"
#include "rwdb.h"
#include <QtSql>
#include<QSqlDatabase>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include "dialoghis.h"
#include "duser.h"
#include "dcard.h"


QTimer *timer ;
void IC_test (void);



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost"); // Đặt tên máy chủ MariaDB của bạn
        db.setPort(3306); // Đặt cổng của MariaDB của bạn
        db.setDatabaseName("nhom3"); // Đặt tên cơ sở dữ liệu của bạn
        db.setUserName("root"); // Đặt tên người dùng MariaDB của bạn
        db.setPassword("1"); // Đặt mật khẩu MariaDB của bạn
        cDB = db.open();
    lcd_init();
    RC522_setup(7);
    PcdReset();
    M500PcdConfigISOType('A');
    printf("Bat dau quet NFC\r\n");
    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(IC_test()));

    timer->start(0);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::IC_test()
{

    char cStr[30];
    uint8_t ucArray_ID[4];    /* LÆ°u trá»¯ loáº¡i IC vÃ  UID (sá»‘ hiá»‡u thá»© tá»± cá»§a IC) cá»§a tháº» IC */
    uint8_t ucStatusReturn;      /* Tráº¡ng thÃ¡i tráº£ vá» */
    char message[100];
    QString cid ;
    QString uid ;
    QString status = "0";


        if ((ucStatusReturn = PcdRequest(PICC_REQALL, ucArray_ID)) != MI_OK) { /* Náº¿u tháº¥t báº¡i, thá»­ tÃ¬m tháº» láº§n ná»¯a */
            ucStatusReturn = PcdRequest(PICC_REQALL, ucArray_ID);
        }

        if (ucStatusReturn == MI_OK) {
            if (PcdAnticoll(ucArray_ID) == MI_OK) {
                sprintf(cStr, "%02X%02X%02X%02X",
                    ucArray_ID[0],
                    ucArray_ID[1],
                    ucArray_ID[2],
                    ucArray_ID[3]);
                qDebug() << "ID la :" << cStr;

                ClrLcd();

                    QSqlQuery query;
                    QString queryStr = QString("SELECT user.name , card.uid, card.cid FROM user JOIN card ON user.uid = card.uid WHERE card.cid = '%1'").arg(QString::fromLatin1(cStr));
                    if (cDB) {
                        if (query.exec(queryStr)) {
                            if (query.next()) {


                                QString name = query.value("Name").toString();
                                cid = query.value("CID").toString();
                                uid = query.value("UID").toString();
                                sprintf(message, "Xin chao %s", name.toUtf8().constData());
                                idcard = cid;

                                query.prepare("INSERT INTO history (CID, UID, status) VALUES (:cid, :uid, :status)");
                                query.bindValue(":cid", cid);
                                query.bindValue(":uid", uid);
                                query.bindValue(":status", status);

                                if (query.exec()) {
                                    whiteLCD(message,LINE1);
                                    whiteLCD("    xin cam on",LINE2);

                                }


                                 qDebug() << "name :" << name;
                                  qDebug() << "name :" << message;


                    }

                else {
                                 qDebug() << "CID khong kkhar dung :" ;
                    whiteLCD("     Loi!!!!",LINE1);
                    whiteLCD("Vui long thu lai",LINE2);
                }
                }



                        delay(2000);

                    ClrLcd();



            }
        }
    }
}



void MainWindow::on_btnUser_clicked()
{
    duser df;
    df.setModal(true);
    df.exec();
}


void MainWindow::on_btnCards_clicked()
{
    dcard cf;
    cf.setModal(true);
    cf.exec();
}


void MainWindow::on_btnHis_clicked()
{
    dialoghis f;
  f.setModal(true);
  f.exec();
  printf("loi");
}



