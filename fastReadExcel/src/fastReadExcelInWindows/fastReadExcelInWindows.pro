#-------------------------------------------------
#
# Project created by QtCreator 2016-07-23T13:23:45
#
#-------------------------------------------------

QT       += core gui
CONFIG += qaxcontainer
QT+=axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fastReadExcelInWindows
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ExcelBase.cpp \
    QVariantListListModel.cpp

HEADERS  += MainWindow.h \
    QVariantListListModel.h \
    ExcelBase.h

FORMS    += MainWindow.ui
