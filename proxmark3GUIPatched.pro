#-------------------------------------------------
#
# Project created by QtCreator 2013-09-13T12:29:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = proxmark3GUIPatched
TEMPLATE = app
CONFIG += link_pkgconfig

PKGCONFIG += libudev
LIBS += -lreadline -lgcc


SOURCES += main.cpp\
        mainwindow.cpp \
    proxcontrol.cpp \
    search_usb.cpp \
    needthings.cpp \
    settings.cpp \
    settingswindow.cpp

HEADERS  += mainwindow.h \
    proxcontrol.h \
    search_usb.h \
    needthings.h \
    settings.h \
    settingswindow.h

FORMS    += mainwindow.ui \
    settingswindow.ui
