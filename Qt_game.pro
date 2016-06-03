#-------------------------------------------------
#
# Project created by QtCreator 2016-04-16T18:21:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_game
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameboard.cpp \
    instructions.cpp

HEADERS  += mainwindow.h \
    gameboard.h \
    instructions.h

FORMS    += mainwindow.ui \
    gameboard.ui \
    instructions.ui

RESOURCES += \
    images.qrc
