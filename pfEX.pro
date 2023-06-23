QT       += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    dcard.cpp \
    dialog.cpp \
    dialoghis.cpp \
    duser.cpp \
    i2clcd.cpp \
    main.cpp \
    mainwindow.cpp \
    rc522.cpp

HEADERS += \
    databasemanager.h \
    dcard.h \
    dialog.h \
    dialoghis.h \
    duser.h \
    i2clcd.h \
    mainwindow.h \
    rc522.h

FORMS += \
    dcard.ui \
    dialog.ui \
    dialoghis.ui \
    duser.ui \
    mainwindow.ui

LIBS += -L/usr/local/lib -lwiringPi
LIBS += -L/usr/lib/arm-linux-gnueabihf/libmariadb.so.3



QT += sql






# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
