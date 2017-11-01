#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QStringList>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
int main()
{

    QString filename = QString("C:\\Users\\Gary Ge\\Pictures\\loan.PNG");
    QFile *data = new QFile(filename);
    qint32 check = data->size();
    data->open(QIODevice::ReadOnly);
    QByteArray blob = data->readLine();
    qint64 poscheck = data->pos();
    QByteArray blob1 = data->readLine();
    qint64 poscheck1 = data->pos();

}
