#-------------------------------------------------
#
# Project created by QtCreator 2017-03-08T14:17:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtThreadTest
TEMPLATE = app


SOURCES += main.cpp\
        Widget.cpp \
    ThreadFromQThread.cpp \
    ThreadObject.cpp

HEADERS  += Widget.h \
    ThreadFromQThread.h \
    ThreadObject.h

FORMS    += Widget.ui
