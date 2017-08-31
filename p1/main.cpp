#include "gygimagewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GYGImageWidget w;
    w.show();

    return a.exec();
}
