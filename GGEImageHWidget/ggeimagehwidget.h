#ifndef GGEIMAGEHWIDGET_H
#define GGEIMAGEHWIDGET_H

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
class GGEImageHWidget : public QWidget
{
    Q_OBJECT

public:
    GGEImageHWidget(LAUMemoryObject image = LAUMemoryObject(), QWidget *parent = 0);
    ~GGEImageHWidget();

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
class GGEImageHDialog : public QDialog
{
    Q_OBJECT

public:
    GGEImageHDialog(LAUMemoryObject image = LAUMemoryObject(), QWidget *parent = 0) : QDialog(parent)
    {
        qDebug() << "HERE";

        this->setLayout(new QVBoxLayout());
        this->layout()->setContentsMargins(6, 6, 6, 6);

        widget = new GGEImageHWidget(image);
        this->layout()->addWidget(widget);

        QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(box->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
        connect(box->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
        this->layout()->addWidget(box);
    }

    ~GGEImageHDialog()
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
    GGEImageHWidget *widget;

signals:
    void emitInt(int);
};

#endif // GGEIMAGEHWIDGET_H
