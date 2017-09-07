#include "gygimagewidget.h"

float h2w;

GYGImageWidget::GYGImageWidget(QImage image,QWidget *parent) : QWidget(parent), label(NULL)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(6,6,6,6);

    //this->setMaximumSize(640, 480);
    if (image.isNull()){
        QSettings settings;
        QString directory = settings.value(QString("GYGImageWidget::lastDirectory"),
                                           QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();;
        //QString directory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).append("/");
            //should have all of this considered at the beginning (settings, gui etc)
        QString filename = QFileDialog::getOpenFileName(this,
                           QString("Select image file to open..."),
                           directory, QString("*.tiff *.jpg"));

        if(filename.isEmpty() == false) {
            QString string = QFileInfo(filename).absolutePath();
            settings.setValue(QString("GYGImageWidget::lastDirectory"), string);

            QImage image(filename);
            ::h2w = (float) image.height()/image.width();
            qDebug() << "FLOAT" << ::h2w << image.height() << image.width() << image.height()/image.width();
            //QImage reimage = image.scaled(image.size(), Qt::KeepAspectRatio,
            //               Qt::FastTransformation);

            if (image.isNull() == false) {
                label = new GYGImageLabel();
                label->onSetImage(image);
                //label->
                //label->setPixmap(QPixmap::fromImage(image));
                //label->setScaledContents(true);
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
    float setWidth = (float) this->width();
    painter.drawImage(QRect(0,0, this->width(), (int) setWidth*::h2w), image);
    painter.drawRect(QRect(0,0, this->width(), (int) setWidth*::h2w));
    painter.end();
}

//QResizeEvent is how to do the homework
