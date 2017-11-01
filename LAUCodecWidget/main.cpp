#include "laucodecwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(QString("Lau Consulting Inc"));
    a.setOrganizationDomain(QString("drhalftone.com"));
    a.setQuitOnLastWindowClosed(true);

    LAUCodecDialog dialog;
    return dialog.exec();
}
