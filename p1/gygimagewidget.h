#ifndef GYGIMAGEWIDGET_H
#define GYGIMAGEWIDGET_H

#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>
#include <QFileDialog>

class GYGImageWidget : public QWidget
{
    Q_OBJECT

public:
    GYGImageWidget(QWidget *parent = 0);
    ~GYGImageWidget();

private:
    QLabel *label;
};

#endif // GYGIMAGEWIDGET_H
