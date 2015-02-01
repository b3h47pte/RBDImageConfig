#-------------------------------------------------
#
# Project created by QtCreator 2015-01-29T20:46:12
#
#-------------------------------------------------

QT       += core gui
CONFIG +=  c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RBDImageConfig
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    Toolbar.cpp \
    Settings.cpp \
    ImageScene.cpp

HEADERS  += MainWindow.h \
    Toolbar.h \
    Settings.h \
    Rectangle.h \
    MultiRectangle.h \
    StringUtility.h \
    ImageScene.h
