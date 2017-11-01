#include "ggeimagehwidget.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(QString("Lau Consulting Inc"));
    a.setOrganizationDomain(QString("drhalftone.com"));
    a.setApplicationName(QString("LAURealSense"));

    QSurfaceFormat format;
    format.setDepthBufferSize(10);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    GGEImageHDialog w;
    if (w.isValid()){
        return w.exec();
    } else {
        return 0;
    }
}
