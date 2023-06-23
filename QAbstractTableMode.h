#ifndef QABSTRACTTABLEMODE_H
#define QABSTRACTTABLEMODE_H
class MyTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    // Các phương thức trừu tượng cần triển khai
    // rowCount(), columnCount(), data(), headerData(), setData(), ...

private:
    // Dữ liệu bảng được lưu trữ ở đây

};


#endif // QABSTRACTTABLEMODE_H
