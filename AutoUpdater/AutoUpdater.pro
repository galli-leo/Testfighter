#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T16:42:40
#
#-------------------------------------------------

QT       += network core gui

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



unix|win32: LIBS += -L$$PWD/zlib/lib/ -lz

INCLUDEPATH += $$PWD/zlib/include
DEPENDPATH += $$PWD/zlib/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/zlib/lib/z.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/zlib/lib/libz.a

unix|win32: LIBS += -L$$PWD/quazip/lib/ -lquazip

INCLUDEPATH += $$PWD/quazip/include
DEPENDPATH += $$PWD/quazip/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/quazip/lib/quazip.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/quazip/lib/libquazip.a
