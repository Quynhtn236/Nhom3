#ifndef RWDB_H
#define RWDB_H

#include <QtSql>

// Struct User
struct User {
    int UID;
    char Name[50];
    char Gender[10];
    char Hometown[50];
    int BirthYear;
};

// Struct Card
struct Card {
    char CID[10];
    int UID;
};

// Struct History
struct History {
    int HID;
    char CID[10];
    int UID;
    QDateTime Timestamp;
    char Status[10];
};


void startDB(void);


class rwDB
{
public:
    rwDB();
};

#endif // RWDB_H
