#include "gygimagewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GYGImageDialog w;

    return w.exec();
}
