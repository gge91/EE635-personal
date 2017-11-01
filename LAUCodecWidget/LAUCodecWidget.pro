#-------------------------------------------------
#
# Project created by QtCreator 2017-09-22T13:16:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LAUCodecWidget
TEMPLATE = app


SOURCES += main.cpp\
        laucodecwidget.cpp

HEADERS  += laucodecwidget.h

INCLUDEPATH += "C:\usr\include"
LIBS += -L"C:\usr\lib" -llibtiff_i
