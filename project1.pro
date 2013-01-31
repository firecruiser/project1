#-------------------------------------------------
#
# Project created by QtCreator 2013-01-27T08:49:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    turret.cpp

HEADERS  += mainwindow.h \
    turret.h

FORMS    += mainwindow.ui

RESOURCES += \
    WindowIcons.qrc
RC_FILE = MainWindow.rc
