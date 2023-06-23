#include "dialoghis.h"
#include "ui_dialoghis.h"
#include <QTableView>
#include <QHeaderView>
#include <QSql>
#include<QSqlDatabase>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

dialoghis::dialoghis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialoghis)
{
    ui->setupUi(this);
    this->setWindowTitle("History");
    QStandardItemModel* model = new QStandardItemModel(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Tự động điều chỉnh kích thước cột
    ui->tableView->verticalHeader()->setVisible(false); // Ẩn tiêu đề dòng
//    model->setRowCount(3); // Số hàng
    model->setColumnCount(5); // Số cột

//    model->setData(model->index(0, 0), "Cell 1"); // Đặt dữ liệu cho ô tại hàng 0, cột 0
//    model->setData(model->index(0, 1), "Cell 2"); // Đặt dữ liệu cho ô tại hàng 0, cột 1
    model->setHeaderData(0, Qt::Horizontal, "ID "); // Đặt tên cho cột 0
    model->setHeaderData(1, Qt::Horizontal, "ID Card");
     model->setHeaderData(2, Qt::Horizontal, "ID User");
      model->setHeaderData(3, Qt::Horizontal, "Status");
       model->setHeaderData(4, Qt::Horizontal, "Time");// Đặt tên cho cột 1
       ui->tableView->setColumnWidth(0, 30);
        ui->tableView->setColumnWidth(4, 400);

    if (m_db.openConnection()) {
        // Kết nối thành công
        QSqlQuery query;

        if (query.exec(" select * from history")) {
            while (query.next()) {
                int row = model->rowCount(); // Lấy số hàng hiện tại trong mô hình
                model->insertRow(row); // Chèn một hàng mới vào mô hình

                // Đặt dữ liệu cho các ô trong hàng mới
                 // Lấy giá trị cột 1
                            model->setData(model->index(row, 0), query.value("HID").toString()); // Lấy giá trị cột 2
                            model->setData(model->index(row, 1), query.value("CID").toString()); // Lấy giá trị cột 3
                            model->setData(model->index(row, 2), query.value("UID").toString()); // Lấy giá trị cột 4
                            model->setData(model->index(row, 3), query.value("status").toString()); // Lấy giá trị cột 5
                            model->setData(model->index(row, 4), query.value("time").toString());
                            ui->tableView->setModel(model);
                            QString hid = query.value("HID").toString();
                                   QString cid = query.value("CID").toString();
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

dialoghis::~dialoghis()
{
    delete ui;
    //m_db.closeConnection();
}
