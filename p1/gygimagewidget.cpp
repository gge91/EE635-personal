#include "gygimagewidget.h"

GYGImageWidget::GYGImageWidget(QWidget *parent) : QWidget(parent), label(NULL)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(6,6,6,6);
    this->setMaximumSize(640, 480);

    QString filename = QFileDialog::getOpenFileName(this, QString("Select image file to open..."), QString(), QString("*.tiff *.jpg"));

    if(filename.isEmpty() == false) {
        QImage image(filename);
        if (image.isNull() == false) {
            label = new QLabel();
            label->setPixmap(QPixmap::fromImage(image));
            this->layout()->addWidget(label);
    }
    }
}


GYGImageWidget::~GYGImageWidget()
{
    ;
}
