#ifndef LAUOPENGLWIDGET_H
#define LAUOPENGLWIDGET_H

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

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit LAUOpenGLWidget(QImage img = QImage(), QWidget *parent = 0) : QOpenGLWidget(parent), image(img), texture(NULL), frameBufferObject(NULL)
    {
        this->setMinimumSize(image.size());
    }
    ~LAUOpenGLWidget();

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

#endif // LAUOPENGLWIDGET_H
