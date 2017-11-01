#ifndef GGEIMAGEWIDGET_H
#define GGEIMAGEWIDGET_H

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

#include "laumemoryobject.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class GGEOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GGEOpenGLWidget(LAUMemoryObject img = LAUMemoryObject(), QWidget *parent = 0) : QOpenGLWidget(parent), image(img), texture(NULL), frameBufferObject(NULL) { ; }
    ~GGEOpenGLWidget();

    bool isValid()
    {
        return (vertexArrayObject.isCreated());
    }

    void grabImage(LAUMemoryObject image);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void onSetImage(LAUMemoryObject img);

private:
    LAUMemoryObject image;
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
class GGEOpenGLHistWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GGEOpenGLHistWidget(LAUMemoryObject img = LAUMemoryObject(), QWidget *parent = 0) : QOpenGLWidget(parent), himage(img), htexture(NULL), hframeBufferObject(NULL) { ; }
    ~GGEOpenGLHistWidget();

    bool isValid()
    {
        return (hvertexArrayObject.isCreated());
    }

    void grabImage(LAUMemoryObject himage);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void onSetImage(LAUMemoryObject img);

private:
    LAUMemoryObject himage;
    QOpenGLFramebufferObject *hframeBufferObject;
    QOpenGLVertexArrayObject hvertexArrayObject;
    QOpenGLBuffer hquadVertexBuffer, hquadIndexBuffer;
    QOpenGLShaderProgram hprogram, hfilter;
    QOpenGLTexture *htexture;

    int localHeight, localWidth;
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class GGEImageWidget : public QWidget
{
    Q_OBJECT

public:
    GGEImageWidget(LAUMemoryObject image = LAUMemoryObject(), QWidget *parent = 0);
    ~GGEImageWidget();

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
class GGEHistWidget : public QWidget
{
    Q_OBJECT

public:
    GGEHistWidget(LAUMemoryObject image = LAUMemoryObject(), QWidget *parent = 0);
    ~GGEHistWidget();

    bool isValid() const
    {
        return (hisValidFlag);
    }

    bool isNull() const
    {
        return (!hisValidFlag);
    }

protected:
    void resizeEvent(QResizeEvent *);

private:
    bool hisValidFlag;
    float haspectRatio;
    GGEOpenGLHistWidget *hlabel;
};
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class GGEImageDialog : public QDialog
{
    Q_OBJECT

public:
    GGEImageDialog(LAUMemoryObject image = LAUMemoryObject(), QWidget *parent = 0) : QDialog(parent)
    {
        qDebug() << "HERE";

        this->setLayout(new QVBoxLayout());
        this->layout()->setContentsMargins(6, 6, 6, 6);

        widget = new GGEImageWidget(image);
        this->layout()->addWidget(widget);

        //hwidget = new GGEHistWidget(image);
        //this->layout()->addWidget(hwidget);

        QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(box->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
        connect(box->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
        this->layout()->addWidget(box);
    }

    ~GGEImageDialog()
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
    GGEImageWidget *widget;
    GGEHistWidget *hwidget;
signals:
    void emitInt(int);
};

#endif // GGEIMAGEWIDGET_H
