#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T16:42:40
#
#-------------------------------------------------

QT       += network core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoUpdater
TEMPLATE = app
INCLUDEPATH -= /Users/leonardogalli/zlib/include
INCLUDEPATH += $$_PRO_FILE_PWD_/include
LIBS += -lz

LIBS += $$_PRO_FILE_PWD_/lib/libquazip.dylib
SOURCES += main.cpp\
        updatingwindow.cpp

HEADERS  += updatingwindow.h

FORMS    += updatingwindow.ui

unix|win32: LIBS += -L$$PWD/lib/ -lquazip

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
