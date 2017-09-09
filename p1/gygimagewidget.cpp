#include "gygimagewidget.h"

GYGImageWidget::GYGImageWidget(QImage image,QWidget *parent) : QWidget(parent), label(NULL)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(6,6,6,6);

    if (image.isNull()){
        QSettings settings;
        QString directory = settings.value(QString("GYGImageWidget::lastDirectory"),
                                           QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();;
        QString filename = QFileDialog::getOpenFileName(this,
                           QString("Select image file to open..."),
                           directory, QString("*.tiff *.jpg"));

        if(filename.isEmpty() == false) {
            QString string = QFileInfo(filename).absolutePath();
            settings.setValue(QString("GYGImageWidget::lastDirectory"), string);

            QImage image(filename);

            if (image.isNull() == false) {
                label = new GYGImageLabel();
                label->onSetImage(image);
                this->layout()->addWidget(label);
            }
        }
    }
}

GYGImageWidget::~GYGImageWidget()
{
    ;
}
void GYGImageLabel::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    painter.setPen(QPen(Qt::black,2));
    painter.drawImage(QRect(0,0, this->width(), this->height()), image);
    painter.drawRect(QRect(0,0, this->width(), this->height()));
    painter.end();
}

void GYGImageWidget::resizeEvent(QResizeEvent *event)
{
    //allows resize then forces aspect ratio
    QWidget::resizeEvent(event);
    float w = this->label->image.width();
    float h = this->label->image.height();
    GYGImageWidget::setFixedHeight((int)(h/w*(this->width())));
}
