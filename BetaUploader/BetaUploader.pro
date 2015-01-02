#-------------------------------------------------
#
# Project created by QtCreator 2014-12-20T14:37:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BetaUploader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    uploadwindow.cpp \
    uploadmanager.cpp

HEADERS  += mainwindow.h \
    uploadwindow.h \
    uploadmanager.h

FORMS    += mainwindow.ui \
    uploadwindow.ui
