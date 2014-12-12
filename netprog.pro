#-------------------------------------------------
#
# Project created by QtCreator 2014-11-14T14:36:14
#
#-------------------------------------------------

QT       += core gui
QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = netprog
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    client.cpp \
    serverworker.cpp \
    clientworker.cpp

HEADERS  += mainwindow.h \
    server.h \
    client.h \
    serverworker.h \
    clientworker.h

FORMS    += mainwindow.ui \
    client.ui \
    server.ui
