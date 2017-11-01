#ifndef GGEHISTOGRAMWIDGET_H
#define GGEHISTOGRAMWIDGET_H

#include <QLabel>
#include <QImage>
#include <QDebug>
#include <QPixmap>
#include <QString>
#include <QWidget>
#include <QPainter>
#include <QSettings>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QResizeEvent>
#include <QStandardPaths>
#include <QDialogButtonBox>

#include <QScreen>
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <laumemoryobject.h>

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class GGEOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GGEOpenGLWidget(QImage img = QImage(), QWidget *parent = 0) : QOpenGLWidget(parent), image(img), texture(NULL), frameBufferObject(NULL){ ; }
    ~GGEOpenGLWidget();

    bool isValid()
    {
        return (vertexArrayObject.isCreated());
    }
    void grabImage(float *buffer);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void onSetImage(QImage img);

private:
    QImage image;
    QOpenGLFramebufferObject *frameBufferObject;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLBuffer quadVertexBuffer, quadIndexBuffer;
    QOpenGLShaderProgram program, filter;
    QOpenGLTexture *texture;

    int localHeight, localWidth;
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class GGEOpenGLHistogram : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GGEOpenGLHistogram(LAUMemoryObject imData = LAUMemoryObject(),QWidget *parent = 0) : QOpenGLWidget(parent), himage(imData), htexture(NULL), frameBufferHist(NULL){
        ;
    }
    ~GGEOpenGLHistogram();

protected:
    void initializeGL();
    void paintGL();
public slots:
    void onSetImage(LAUMemoryObject imData);

private:
    LAUMemoryObject holder;
    LAUMemoryObject himage;
    QOpenGLTexture *htexture;
    QOpenGLShaderProgram hprogram,hfilter;
    QOpenGLFramebufferObject *frameBufferHist;
    QOpenGLVertexArrayObject hvertexArrayObject;
    QOpenGLBuffer hquadVertexBuffer, hquadIndexBuffer;
    //QOpenGLTexture *texture;
    int localHeight, localWidth;
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class GGEHistogramImage : public QWidget //widget that contains the histogram image
{
    Q_OBJECT

public:
    GGEHistogramImage(LAUMemoryObject himage = LAUMemoryObject(), QWidget *parent = 0);
    ~GGEHistogramImage();

    bool isValid() const
    {
        return (isValidFlag);
    }

    bool isNull() const
    {
        return (!isValidFlag);
    }

protected:
    void resizeEvent(QResizeEvent *);

private:
    bool isValidFlag;
    float aspectRatio;
    GGEOpenGLWidget *label;
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class GGEHistogramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GGEHistogramWidget(LAUMemoryObject image = LAUMemoryObject(), QWidget *parent = 0);
    ~GGEHistogramWidget();

    bool isValid() const
    {
        return (isValidFlag);
    }

    bool isNull() const
    {
        return (!isValidFlag);
    }

protected:
    void resizeEvent(QResizeEvent *);

private:
    bool isValidFlag;
    float aspectRatio;
    GGEOpenGLWidget *label;
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class GGEHistogramDialog : public QDialog
{
    Q_OBJECT

public:
    GGEHistogramDialog(LAUMemoryObject image = LAUMemoryObject(), LAUMemoryObject himage = LAUMemoryObject(), QWidget *parent = 0) : QDialog(parent)
    {
        qDebug() << "HERE";

        this->setLayout(new QVBoxLayout());
        this->layout()->setContentsMargins(6, 6, 6, 6);

        widget = new GGEHistogramWidget(image);
        this->layout()->addWidget(widget);

        hwidget = new GGEHistogramImage(himage); //add a small widget to display histogram
        this->layout()->addWidget(hwidget);

        QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(box->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
        connect(box->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
        this->layout()->addWidget(box);
    }

    ~GGEHistogramDialog()
    {
        ;
    }

    bool isValid() const
    {
        return (widget->isValid());
    }

    bool isNull() const
    {
        return (widget->isNull());
    }

public slots:
    void onInt(int r)
    {
        qDebug() << "INT" << r;
    }

protected:
    void accept()
    {
        QDialog::accept();
    }

    void reject()
    {
        QDialog::reject();
    }

    void resizeEvent(QResizeEvent *);

private:
    GGEHistogramWidget *widget;
    GGEHistogramImage *hwidget;
signals:
    void emitInt(int);
};

#endif // GGEHISTOGRAMWIDGET_H
