#ifndef LAUCONVOLUTIONGLCONTEXT_H
#define LAUCONVOLUTIONGLCONTEXT_H

#include <QObject>
#include <QThread>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLVertexArrayObject>

#include "laumemoryobject.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUConvolutionGLContext : public QOpenGLContext, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit LAUConvolutionGLContext(LAUMemoryObject objA, LAUMemoryObject objB, QWidget *parent = NULL);
    ~LAUConvolutionGLContext();

    void result(LAUMemoryObject object);

private:
    LAUMemoryObject objectA, objectB;

    QSurface *surface;
    QOpenGLShaderProgram program;
    QOpenGLBuffer quadVertexBuffer, quadIndexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLFramebufferObject *frameBufferObject;
    QOpenGLTexture *textureA, *textureB;

    QOpenGLTexture setTexture(LAUMemoryObject obj);
    void initialize();
    void filter();
};

#endif // LAUCONVOLUTIONGLCONTEXT_H
