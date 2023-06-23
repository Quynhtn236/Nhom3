#include "rwdb.h"
#include <QtSql>
#include <QSqlDatabase>



// them user

// sua user

// xoa user

// them the*

// xoa the*

//xem lich su*

// quet the them lich su*


void startDB(void){

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost"); // Đặt tên máy chủ MariaDB của bạn
    db.setPort(3306); // Đặt cổng của MariaDB của bạn
    db.setDatabaseName("nhom3"); // Đặt tên cơ sở dữ liệu của bạn
    db.setUserName("root"); // Đặt tên người dùng MariaDB của bạn
    db.setPassword("1"); // Đặt mật khẩu MariaDB của bạn

    if (db.open()) {
        // Kết nối thành công
        qDebug() << "Kết noi thNH CONG " << db.lastError().text();
        QSqlQuery query;
        if (query.exec("SELECT * FROM user")) {
            while (query.next()) {
                QString name = query.value("Name").toString();
                qDebug() << "Tên: " << name;
            }
        } else {
            qDebug() << "Lỗi truy vấn: " << query.lastError().text();
        }


    } else {
        // Kết nối thất bại
        qDebug() << "Kết nối đến cơ sở dữ liệu thất bại: " << db.lastError().text();
    }

}





