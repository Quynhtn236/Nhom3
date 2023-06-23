#include "dcard.h"
#include "ui_dcard.h"
#include <QSql>
#include<QSqlDatabase>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>


dcard::dcard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dcard)
{
    ui->setupUi(this);
    this->setWindowTitle("Card");
    resetTable();
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &dcard::onSelectionChanged);
}

void dcard::resetTable()
{
    QStandardItemModel* model = new QStandardItemModel(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Tự động điều chỉnh kích thước cột
    ui->tableView->verticalHeader()->setVisible(false); // Ẩn tiêu đề dòng
//    model->setRowCount(3); // Số hàng
    model->setColumnCount(3); // Số cột

//    model->setData(model->index(0, 0), "Cell 1"); // Đặt dữ liệu cho ô tại hàng 0, cột 0
//    model->setData(model->index(0, 1), "Cell 2"); // Đặt dữ liệu cho ô tại hàng 0, cột 1
    model->setHeaderData(0, Qt::Horizontal, "ID User "); // Đặt tên cho cột 0
    model->setHeaderData(1, Qt::Horizontal, "Name");
     model->setHeaderData(2, Qt::Horizontal, "ID Card");

//       ui->tableView->setColumnWidth(0, 30);
//        ui->tableView->setColumnWidth(4, 400);
     //   ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    if (m_db.openConnection()) {
        // Kết nối thành công
        QSqlQuery query;

        if (query.exec("SELECT user.uid, user.name, card.cid FROM user LEFT JOIN card ON user.uid = card.uid")) {
            while (query.next()) {
                int row = model->rowCount(); // Lấy số hàng hiện tại trong mô hình
                model->insertRow(row); // Chèn một hàng mới vào mô hình

                // Đặt dữ liệu cho các ô trong hàng mới
                 // Lấy giá trị cột 1
                            model->setData(model->index(row, 0), query.value("UID").toString()); // Lấy giá trị cột 2
                            model->setData(model->index(row, 1), query.value("name").toString()); // Lấy giá trị cột 3
                            model->setData(model->index(row, 2), query.value("CID").toString()); // Lấy giá trị cột 4

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

void dcard::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    // Kiểm tra nếu có dòng được chọn
    if (!selected.isEmpty()) {
        // Lấy chỉ mục của dòng được chọn
        QModelIndex index = selected.indexes().at(0);

        // Lấy dữ liệu từ các cột của dòng được chọn
        QVariant data = index.sibling(index.row(), 2).data(); // Lấy dữ liệu từ cột 0


        // Xử lý dữ liệu của dòng được chọn
        ui->ledelete->setText(data.toString());
    }
}

dcard::~dcard()
{
    delete ui;
}

void dcard::on_pushButton_clicked()
{

}

void dcard::on_btnAdd_clicked()
{
    QString UID = ui->leUID->text();
    QString CID = ui->leCID->text();

    qDebug() << "UID = " << UID;
    qDebug() << "CID = " << CID;

    QSqlQuery query;
   QString queryStr = QString("SELECT * FROM user left join card on user.uid = card.uid where user.uid = '%1' and card.uid is null ").arg(UID);
   QString queryStr1 = QString("SELECT * FROM user where uid = '%1'  ").arg(UID);
    QString queryStr2 = QString("SELECT * FROM card where cid = '%1'  ").arg(CID);
    if (query.exec(queryStr1)) {

        if (query.next()) {
            if (query.exec(queryStr)) {

                if (query.next()) {
                    if (query.exec(queryStr2)) {

                        if (query.next()) {
                            QMessageBox::critical(this, "ERROR", "Card already  exist");
                        }
                        else{
                            if(CID.isEmpty()){
                                QMessageBox::critical(this, "ERROR", "CID cannot be null");

                            }
                            else{
                                QString queryStr3 = QString("INSERT INTO card (cid, uid) VALUES ('%1', '%2')").arg(CID).arg(UID);
                                if (query.exec(queryStr3)) {
                                    resetTable();
                                        QMessageBox::critical(this, "SUCCESSFUL", "Registration successful.");
                                    }
                            }

                        }

                    }

                }
                else{
                    QMessageBox::critical(this, "ERROR", "User does not exist");
                }

            }
        }
        else{
            QMessageBox::critical(this, "ERROR", "User does not exist");
        }

    } else {
        qDebug() << "Lỗi SQL:" << query.lastError().text();

    }


}

void dcard::on_btnDel_clicked()
{
    QString CID = ui->ledelete->text();
    if(CID.isEmpty()){
         QMessageBox::critical(this, "ERROR", "CID cannot be null ");

    }
    else{
        QSqlQuery query;
        QString queryStr1 = QString("select * from card where cid = '%1'").arg(CID);
        QString queryStr2 = QString("UPDATE history SET cid = NULL WHERE cid = '%1'").arg(CID);
        QString queryStr3 = QString("DELETE FROM card  WHERE cid = '%1'").arg(CID);
        if (query.exec(queryStr1)) {
            if (query.exec(queryStr2)) {
                if (query.exec(queryStr3)) {
                    resetTable();
                    QMessageBox::critical(this, "SUCCESSFUL", "CID delete done ");

                    }
                else{
                    QMessageBox::critical(this, "ERROR", "delete card fail ");
                }
                }
            else{
                QMessageBox::critical(this, "ERROR", "update history error ");
            }
            }
        else{
            QMessageBox::critical(this, "ERROR", "CID does not exit ");
        }
    }

}
