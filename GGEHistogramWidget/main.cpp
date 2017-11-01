#include "ggehistogramwidget.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(10);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    GGEHistogramDialog w;
    if (w.isValid()){
        return w.exec();
    } else {
        return 0;
    }
}
