#include "duser.h"
#include "ui_duser.h"
#include <QTableView>
#include <QHeaderView>
#include <QSql>
#include<QSqlDatabase>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

duser::duser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::duser)
{
    ui->setupUi(this);
   this->setWindowTitle("User");

    QStandardItemModel* model = new QStandardItemModel(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Tự động điều chỉnh kích thước cột
    ui->tableView->verticalHeader()->setVisible(false); // Ẩn tiêu đề dòng
//    model->setRowCount(3); // Số hàng
    model->setColumnCount(5); // Số cột

//    model->setData(model->index(0, 0), "Cell 1"); // Đặt dữ liệu cho ô tại hàng 0, cột 0
//    model->setData(model->index(0, 1), "Cell 2"); // Đặt dữ liệu cho ô tại hàng 0, cột 1
    model->setHeaderData(0, Qt::Horizontal, "ID "); // Đặt tên cho cột 0
    model->setHeaderData(1, Qt::Horizontal, "ID User");
    model->setHeaderData(2, Qt::Horizontal, "Name");
    model->setHeaderData(3, Qt::Horizontal, "Gender");
    model->setHeaderData(4, Qt::Horizontal, "Birth Year");

    ui->tableView->setColumnWidth(4, 400);

    if (m_db.openConnection()) {
        // Kết nối thành công
        QSqlQuery query;
        int count = 0;

        if (query.exec(" select * from user")) {
            while (query.next()) {
                int row = model->rowCount(); // Lấy số hàng hiện tại trong mô hình
                model->insertRow(row); // Chèn một hàng mới vào mô hình
                count ++;
                // Đặt dữ liệu cho các ô trong hàng mới
                 // Lấy giá trị cột 1
                            model->setData(model->index(row, 0), count); // Lấy giá trị cột 2
                            model->setData(model->index(row, 1), query.value("UID").toString()); // Lấy giá trị cột 3
                            model->setData(model->index(row, 2), query.value("name").toString()); // Lấy giá trị cột 4
                            model->setData(model->index(row, 3), query.value("gender").toString()); // Lấy giá trị cột 5
                            model->setData(model->index(row, 4), query.value("birth_year").toString());
                            ui->tableView->setModel(model);
//                            QString hid = query.value("HID").toString();
//                                   QString cid = query.value("CID").toString();
                                   // ...

//                                   // In giá trị để kiểm tra
//                                   qDebug() << "HID:" << hid;
//                                   qDebug() << "CID:" << cid;
            }
        } else {
            qDebug() << "Lỗi SQL:" << query.lastError().text();

        }


    } else {

    }
    // ...
    ui->tableView->setModel(model);
}

duser::~duser()
{
    delete ui;
}
