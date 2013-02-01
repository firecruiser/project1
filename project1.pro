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


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/hidapi/windows/release/ -lhidapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/hidapi/windows/debug/ -lhidapi
else:unix: LIBS += -L$$PWD/hidapi/windows/ -lhidapi

INCLUDEPATH += $$PWD/hidapi/windows/Debug
DEPENDPATH += $$PWD/hidapi/windows/Debug
