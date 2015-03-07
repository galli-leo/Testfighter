#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T16:42:40
#
#-------------------------------------------------

QT       += network core gui

#DEFINES +=ZLIB_WINAPI=“1”
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoUpdater
TEMPLATE = app

INCLUDEPATH += $$PWD/../shared/

SOURCES += main.cpp\
        updatingwindow.cpp \
    ../shared/appdata.cpp

HEADERS  += updatingwindow.h \
    ../shared/appdata.h


FORMS    += updatingwindow.ui

DISTFILES += \
    ../shared/settings.json \
    ../shared/7za.exe


