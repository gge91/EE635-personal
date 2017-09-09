#ifndef GYGIMAGEWIDGET_H
#define GYGIMAGEWIDGET_H


#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QString>
#include <QSize>
#include <QWidget>
#include <QSettings>
#include <QDialogButtonBox>
#include <QResizeEvent>
#include <QPushButton>
#include <QPainter>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSizePolicy>


class GYGImageObject
{
    ;
};

class GYGImageLabel: public QLabel
{
    Q_OBJECT


public:
    GYGImageLabel(QWidget *parent = 0) : QLabel(parent) {;}
    ~GYGImageLabel() {;}
    QImage image; //made QImage public to grab image properties with QWidget::resizeEvent

public slots:
    void onSetImage(QImage img)
    {
        image = img;
        update();
    }

protected:
    void paintEvent(QPaintEvent *);

//private:
};


class GYGImageWidget : public QWidget
{
    Q_OBJECT

public:
    GYGImageWidget(QImage image = QImage(), QWidget *parent=0);
    ~GYGImageWidget();

private:
    GYGImageLabel *label;

protected:
    void resizeEvent(QResizeEvent *event); //hw1 resize declaration

};

class GYGImageDialog : public QDialog
{
    Q_OBJECT

public:
    GYGImageDialog(QWidget *parent = 0) : QDialog(parent)
    {
        this->setLayout(new QVBoxLayout());
        this->layout()->setContentsMargins(6,6,6,6);

        widget = new GYGImageWidget();
        this->layout()->addWidget(widget);

        QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
        connect(box->button(QDialogButtonBox::Ok), SIGNAL(clicked()),this,SLOT(accept()));
        connect(box->button(QDialogButtonBox::Cancel),SIGNAL(clicked()),this,SLOT(reject()));
        this->layout()->addWidget(box);

        //connect(this, SIGNAL(emitInt(int)),this, SLOT(onInt(int)),Qt::QueuedConnection);
    }
    ~GYGImageDialog()
    {
        ;
    }
public slots:
    void onInt(int r)
    {
        qDebug() << "INT" << r;
    }

protected:
    void accept()
    {
        /*this is a way to intercept built in functions and instead do something else with it
        for example you may want to save something done when accept is hit in the dialog box
        you can do it here
        also note that when this is done the arguments have to be the same (in this case accept() has no
        arguments so the void also has no arguments)*/
        //save results
        QDialog::accept();
    }
    void reject()
    {
        QDialog::reject();
    }

private:
    GYGImageWidget *widget;

signals:
    void emitInt(int);
};
#endif // GYGIMAGEWIDGET_H
