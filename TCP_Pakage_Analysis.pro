#-------------------------------------------------
#
# Project created by QtCreator 2017-04-19T23:26:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCP_Pakage_Analysis
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    package.cpp \
    choosedevice.cpp \
    listener.cpp


HEADERS  += mainwindow.h \
    package.h \
    choosedevice.h \
    listener.h


FORMS    += mainwindow.ui \
    choosedevice.ui

CONFIG +=C++11
INCLUDEPATH +=D:/wincap/Include


LIBS += -LD:/wincap/Lib/ -lPacket

LIBS += -LD:/wincap/Lib/ -lwpcap

RESOURCES += \
    image.qrc

RC_FILE =icon.rc
