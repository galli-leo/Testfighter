#-------------------------------------------------
#
# Project created by QtCreator 2014-12-13T17:28:58
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BetaLauncher
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filedownloader.cpp

HEADERS  += mainwindow.h \
    filedownloader.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    button.css \
    blue_button02.png \
    blue_button03.png

RESOURCES += \
    Resources.qrc
