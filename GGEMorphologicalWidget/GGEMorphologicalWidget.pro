#-------------------------------------------------
#
# Project created by QtCreator 2017-10-31T18:10:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GGEMorphologicalWidget
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        ggemorphologicalwidget.cpp\
        laumemoryobject.cpp

HEADERS += \
        ggemorphologicalwidget.h\
        laumemoryobject.h

win32 {
    LIBS += -lopengl32
}

DISTFILES += \
    lauimagewidget.vert \
    lauimagewidget.frag \
    lauimagefilter.frag \
    lauconvolutionfilter.vert \
    lauconvolutionfilter.frag \
    ggemorphologicalwidget.vert \
    ggemorphologicalwidget.frag \
    ggemorphopen.vert \
    ggemorphopen.frag \
    ggemorphclose.frag

RESOURCES += \
    lauimagewidget.qrc \
    ggemorphologicalwidget.qrc

win32 {
    INCLUDEPATH += $$quote(C:/usr/include)
    DEPENDPATH  += $$quote(C:/usr/include)
    LIBS        += -L$$quote(C:/usr/lib) -llibtiff_i -lopengl32
}
