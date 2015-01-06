#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T16:42:40
#
#-------------------------------------------------

QT       += network core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoUpdater
TEMPLATE = app
INCLUDEPATH += $$PWD/zlib/include


unix|win32: LIBS += $$PWD/zlib/lib/zlib.lib
unix|win32: LIBS += $$PWD/quazip/lib/quazip.lib

INCLUDEPATH += $$PWD/quazip/include


unix:LIBS += -lz


SOURCES += main.cpp\
        updatingwindow.cpp

HEADERS  += updatingwindow.h

FORMS    += updatingwindow.ui


