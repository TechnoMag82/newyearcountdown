#-------------------------------------------------
#
# Project created by QtCreator 2021-07-21T11:28:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = NewYearCountdown
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    Settings/settings.cpp \
    mycalendar.cpp \
    updatelefttimethread.cpp

HEADERS  += mainwindow.h \
    Settings/settings.h \
    mycalendar.h \
    updatelefttimethread.h

RESOURCES += \
    appresources.qrc

