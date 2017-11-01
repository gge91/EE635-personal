#-------------------------------------------------
#
# Project created by QtCreator 2017-10-11T19:31:17
#
#-------------------------------------------------

QT       += core gui opengl widgets

TARGET = GGEImageWidget
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
        ggeimagewidget.cpp \
        laumemoryobject.cpp

HEADERS += \
        ggeimagewidget.h \
        laumemoryobject.h

RESOURCES += \
    ggeimagewidget.qrc

win32 {
    LIBS += -lopengl32
}

DISTFILES += \
    ggeimagewidget.vert \
    ggeimagewidget.frag \
    ggehistcalc.frag \
    ggehistcalc.vert \
    ggehistdisp.frag \
    ggehistdisp.vert \
    ggehistcalc.geom

win32 {
    INCLUDEPATH += $$quote(C:/usr/include)
    DEPENDPATH  += $$quote(C:/usr/include)
    LIBS        += -L$$quote(C:/usr/lib) -llibtiff_i -lopengl32
}
