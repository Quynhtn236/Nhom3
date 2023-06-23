#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
}

bool DatabaseManager::openConnection()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("localhost"); // Địa chỉ máy chủ MariaDB
    m_db.setPort(3306); // Cổng mặc định của MariaDB
    m_db.setDatabaseName("nhom3"); // Tên cơ sở dữ liệu
    m_db.setUserName("root"); // Tên đăng nhập MariaDB
    m_db.setPassword("1"); // Mật khẩu MariaDB

    if (!m_db.open()) {
        qDebug() << "Lỗi kết nối cơ sở dữ liệu:";
        qDebug() << m_db.lastError().text();
        return false;
    }

    return true;
}

void DatabaseManager::closeConnection()
{
    m_db.close();
}

QSqlDatabase& DatabaseManager::getDatabase()
{
    return m_db;
}
