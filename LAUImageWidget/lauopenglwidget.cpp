#include "lauopenglwidget.h"
#include "locale.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUOpenGLWidget::~LAUOpenGLWidget()
{
    if (isValid()) {
        makeCurrent();
        if (texture) {
            delete texture;
        }
        if (frameBufferObject) {
            delete frameBufferObject;
        }
        vertexArrayObject.release();
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUOpenGLWidget::onSetImage(QImage img)
{
    image = img;

    if (isValid() && image.isNull() == false) {
        // MAKE THIS THE CURRENT OPENGL CONTEXT
        makeCurrent();

        if (frameBufferObject) {
            delete frameBufferObject;
        }
        QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
        frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);
        frameBufferObject = new QOpenGLFramebufferObject(image.size(), frameBufferObjectFormat);

        if (texture) {
            delete texture;
        }
        texture = new QOpenGLTexture(image);

        // CLEAR THE FRAME BUFFER OBJECT
        glViewport(0, 0, frameBufferObject->width(), frameBufferObject->height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (frameBufferObject->bind()) {
            if (filter.bind()) {
                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE0);
                        texture->bind();
                        filter.setUniformValue("qt_texture", 0);
                        filter.setUniformValue("qt_flip", true);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(filter.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        filter.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                filter.release();
            }
            frameBufferObject->release();
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUOpenGLWidget::grabImage(float *buffer)
{
    if (buffer && frameBufferObject) {
        makeCurrent();
        glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, (unsigned char *)buffer);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUOpenGLWidget::initializeGL()
{
    // BRIDGE BETWEEN QT OPENGL FUNCTIONS AND THOSE RUNNING ON HOST COMPUTER
    initializeOpenGLFunctions();

    // get context opengl-version
    qDebug() << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
    qDebug() << "Context valid: " << context()->isValid();
    qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "OpenGl information: VENDOR:       " << (const char *)glGetString(GL_VENDOR);
    qDebug() << "                    RENDERDER:    " << (const char *)glGetString(GL_RENDERER);
    qDebug() << "                    VERSION:      " << (const char *)glGetString(GL_VERSION);
    qDebug() << "                    GLSL VERSION: " << (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

    // SET THE CLEAR COLOR
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // CREATE THE VERTEX ARRAY OBJECT FOR FEEDING VERTICES TO OUR SHADER PROGRAMS
    vertexArrayObject.create();
    vertexArrayObject.bind();

    // CREATE A BUFFER TO HOLD THE ROW AND COLUMN COORDINATES OF IMAGE PIXELS FOR THE TEXEL FETCHES
    quadVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    quadVertexBuffer.create();
    quadVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (quadVertexBuffer.bind()) {
        quadVertexBuffer.allocate(16 * sizeof(float));
        float *vertices = (float *)quadVertexBuffer.map(QOpenGLBuffer::WriteOnly);
        if (vertices) {
            vertices[0]  = -1.0;
            vertices[1]  = -1.0;
            vertices[2]  = 0.0;
            vertices[3]  = 1.0;
            vertices[4]  = +1.0;
            vertices[5]  = -1.0;
            vertices[6]  = 0.0;
            vertices[7]  = 1.0;
            vertices[8]  = +1.0;
            vertices[9]  = +1.0;
            vertices[10] = 0.0;
            vertices[11] = 1.0;
            vertices[12] = -1.0;
            vertices[13] = +1.0;
            vertices[14] = 0.0;
            vertices[15] = 1.0;

            quadVertexBuffer.unmap();
        } else {
            qDebug() << QString("Unable to map quadVertexBuffer from GPU.");
        }
    }

    // CREATE AN INDEX BUFFER FOR THE INCOMING DEPTH VIDEO DRAWN AS POINTS
    quadIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    quadIndexBuffer.create();
    quadIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (quadIndexBuffer.bind()) {
        quadIndexBuffer.allocate(6 * sizeof(unsigned int));
        unsigned int *indices = (unsigned int *)quadIndexBuffer.map(QOpenGLBuffer::WriteOnly);
        if (indices) {
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 0;
            indices[4] = 2;
            indices[5] = 3;
            quadIndexBuffer.unmap();
        } else {
            qDebug() << QString("quadIndexBuffer buffer mapped from GPU.");
        }
    }

    // CREATE GLSL PROGRAM FOR PROCESSING THE INCOMING VIDEO
    setlocale(LC_NUMERIC, "C");
    filter.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lauimagewidget.vert");
    filter.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/lauimagefilter.frag");
    filter.link();

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lauimagewidget.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/lauimagewidget.frag");
    program.link();
    setlocale(LC_ALL, "");

    // UPLOAD IMAGE TO TEXTURE
    if (image.isNull() == false) {
        onSetImage(image);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUOpenGLWidget::resizeGL(int w, int h)
{
    // Get the Desktop Widget so that we can get information about multiple monitors connected to the system.
    QDesktopWidget *dkWidget = QApplication::desktop();
    QList<QScreen *> screenList = QGuiApplication::screens();
    qreal devicePixelRatio = screenList[dkWidget->screenNumber(this)]->devicePixelRatio();
    localHeight = h * devicePixelRatio;
    localWidth = w * devicePixelRatio;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUOpenGLWidget::paintGL()
{
    // CLEAR THE FRAME BUFFER OBJECT
    glViewport(0, 0, localWidth, localHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (texture) {
        if (program.bind()) {
            // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
            if (quadVertexBuffer.bind()) {
                if (quadIndexBuffer.bind()) {
                    // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
                    program.setUniformValue("qt_texture", 1);
                    program.setUniformValue("qt_flip", false);

                    // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                    glVertexAttribPointer(program.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                    program.enableAttributeArray("qt_vertex");
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                    quadIndexBuffer.release();
                }
                quadVertexBuffer.release();
            }
            program.release();
        }
    }
}
