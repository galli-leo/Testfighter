#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T16:42:40
#
#-------------------------------------------------

QT       += network core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoUpdater
TEMPLATE = app
CONFIG += staticlib
INCLUDEPATH += $$PWD/zlib/include
DEFINES += QUAZIP_STATIC

unix|win32: LIBS += -L$$PWD/zlib/lib/ -lzlib
unix|win32: LIBS += -L$$PWD/quazip/lib -lquazip

INCLUDEPATH += $$PWD/quazip/include


unix:LIBS += -lz


SOURCES += main.cpp\
        updatingwindow.cpp

HEADERS  += updatingwindow.h

FORMS    += updatingwindow.ui


