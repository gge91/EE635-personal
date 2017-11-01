#include "ggeimagewidget.h"
#include "locale.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
GGEOpenGLWidget::~GGEOpenGLWidget()
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
}/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
GGEOpenGLHistWidget::~GGEOpenGLHistWidget()
{
    if (isValid()) {
        makeCurrent();
        if (htexture) {
            delete htexture;
        }
        if (hframeBufferObject) {
            delete hframeBufferObject;
        }
        hvertexArrayObject.release();
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEOpenGLWidget::onSetImage(LAUMemoryObject img)
{
    image = img;

    if (isValid() && image.isNull() == false && image.colors() <= 4) {
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
        texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        texture->setSize(image.width(), image.height());
        texture->setFormat(QOpenGLTexture::RGBA32F);
        texture->setWrapMode(QOpenGLTexture::ClampToBorder);
        texture->setMinificationFilter(QOpenGLTexture::Nearest);
        texture->setMagnificationFilter(QOpenGLTexture::Nearest);
        texture->allocateStorage();
        if (image.colors() == 1){
            if (image.depth() == sizeof(unsigned char)){
                texture->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, (const void *)image.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                texture->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt16, (const void *)image.constPointer());
            } else if (image.depth() == sizeof(float)){
                texture->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, (const void *)image.constPointer());
            }
        } else if (image.colors() == 2){
            if (image.depth() == sizeof(unsigned char)){
                texture->setData(QOpenGLTexture::RG, QOpenGLTexture::UInt8, (const void *)image.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                texture->setData(QOpenGLTexture::RG, QOpenGLTexture::UInt16, (const void *)image.constPointer());
            } else if (image.depth() == sizeof(float)){
                texture->setData(QOpenGLTexture::RG, QOpenGLTexture::Float32, (const void *)image.constPointer());
            }
        } else if (image.colors() == 3){
            if (image.depth() == sizeof(unsigned char)){
                texture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)image.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                texture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt16, (const void *)image.constPointer());
            } else if (image.depth() == sizeof(float)){
                texture->setData(QOpenGLTexture::RGB, QOpenGLTexture::Float32, (const void *)image.constPointer());
            }
        } else if (image.colors() == 4){
            if (image.depth() == sizeof(unsigned char)){
                texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (const void *)image.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt16, (const void *)image.constPointer());
            } else if (image.depth() == sizeof(float)){
                texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::Float32, (const void *)image.constPointer());
            }
        }

        // CLEAR THE FRAME BUFFER OBJECT
        glViewport(0, 0, frameBufferObject->width(), frameBufferObject->height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (frameBufferObject->bind()) {
            if (program.bind()) {
                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE0);
                        texture->bind();
                        program.setUniformValue("qt_texture", 0);
                        program.setUniformValue("qt_flip", true);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(filter.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        program.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                program.release();
            }
            frameBufferObject->release();
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEOpenGLWidget::grabImage(LAUMemoryObject image)
{
    if (frameBufferObject && image.width() == frameBufferObject->width() && image.height() == frameBufferObject->height()) {
        makeCurrent();
        glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
        if (image.colors() == 1){
            if (image.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, (unsigned char *)image.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_SHORT, (unsigned char *)image.constPointer());
            } else if (image.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, (unsigned char *)image.constPointer());
            }
        } else if (image.colors() == 2){
            if (image.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_UNSIGNED_BYTE, (unsigned char *)image.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_UNSIGNED_SHORT, (unsigned char *)image.constPointer());
            } else if (image.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, (unsigned char *)image.constPointer());
            }
        } else if (image.colors() == 3){
            if (image.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)image.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_SHORT, (unsigned char *)image.constPointer());
            } else if (image.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, (unsigned char *)image.constPointer());
            }
        } else if (image.colors() == 4){
            if (image.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *)image.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_SHORT, (unsigned char *)image.constPointer());
            } else if (image.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (unsigned char *)image.constPointer());
            }
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEOpenGLWidget::initializeGL()
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
    filter.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ggeimagewidget.vert");
    filter.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ggeimagewidget.frag");
    filter.link();

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ggeimagewidget.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ggeimagewidget.frag");
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
void GGEOpenGLWidget::resizeGL(int w, int h)
{
    // Get the Desktop Widget so that we can get information about multiple monitors connected to the system.
    QDesktopWidget *dkWidget = QApplication::desktop();
    QList<QScreen *> screenList = QGuiApplication::screens();
    qreal devicePixelRatio = screenList[dkWidget->screenNumber(this)]->devicePixelRatio();
    localHeight = h * devicePixelRatio;
    localWidth = w * devicePixelRatio;

    qDebug() << localWidth << localHeight << w << h;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEOpenGLWidget::paintGL()
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
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEOpenGLHistWidget::onSetImage(LAUMemoryObject img)
{
    himage = img;

    if (isValid() && himage.isNull() == false && himage.colors() <= 4) {
        // MAKE THIS THE CURRENT OPENGL CONTEXT
        makeCurrent();

        if (hframeBufferObject) {
            delete hframeBufferObject;
        }
        QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
        frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);
        hframeBufferObject = new QOpenGLFramebufferObject(4,256, frameBufferObjectFormat);

        if (htexture) {
            delete htexture;
        }
        htexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        htexture->setSize(himage.width(), himage.height());
        htexture->setFormat(QOpenGLTexture::RGBA32F);
        htexture->setWrapMode(QOpenGLTexture::ClampToBorder);
        htexture->setMinificationFilter(QOpenGLTexture::Nearest);
        htexture->setMagnificationFilter(QOpenGLTexture::Nearest);
        htexture->allocateStorage();
        if (himage.colors() == 1){
            if (himage.depth() == sizeof(unsigned char)){
                htexture->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, (const void *)himage.constPointer());
            } else if (himage.depth() == sizeof(unsigned short)){
                htexture->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt16, (const void *)himage.constPointer());
            } else if (himage.depth() == sizeof(float)){
                htexture->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, (const void *)himage.constPointer());
            }
        } else if (himage.colors() == 2){
            if (himage.depth() == sizeof(unsigned char)){
                htexture->setData(QOpenGLTexture::RG, QOpenGLTexture::UInt8, (const void *)himage.constPointer());
            } else if (himage.depth() == sizeof(unsigned short)){
                htexture->setData(QOpenGLTexture::RG, QOpenGLTexture::UInt16, (const void *)himage.constPointer());
            } else if (himage.depth() == sizeof(float)){
                htexture->setData(QOpenGLTexture::RG, QOpenGLTexture::Float32, (const void *)himage.constPointer());
            }
        } else if (himage.colors() == 3){
            if (himage.depth() == sizeof(unsigned char)){
                htexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)himage.constPointer());
            } else if (himage.depth() == sizeof(unsigned short)){
                htexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt16, (const void *)himage.constPointer());
            } else if (himage.depth() == sizeof(float)){
                htexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::Float32, (const void *)himage.constPointer());
            }
        } else if (himage.colors() == 4){
            if (himage.depth() == sizeof(unsigned char)){
                htexture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (const void *)himage.constPointer());
            } else if (himage.depth() == sizeof(unsigned short)){
                htexture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt16, (const void *)himage.constPointer());
            } else if (himage.depth() == sizeof(float)){
                htexture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::Float32, (const void *)himage.constPointer());
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ONE);
        glBlendEquation(GL_FUNC_ADD);
        glPointSize(1.0f);

        // CLEAR THE FRAME BUFFER OBJECT
        glViewport(0, 0, hframeBufferObject->width(), hframeBufferObject->height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (hframeBufferObject->bind()) {
                        if (hfilter.bind()) {
                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (hquadVertexBuffer.bind()) {
                    if (hquadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE0);
                        htexture->bind();
                        hfilter.setUniformValue("qt_texture", 0);
                        hfilter.setUniformValue("qt_flip", true);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(hfilter.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        hfilter.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_POINTS, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        hquadIndexBuffer.release();
                    }
                    hquadVertexBuffer.release();
                }
                hfilter.release();
            }
            hframeBufferObject->release();
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEOpenGLHistWidget::grabImage(LAUMemoryObject image)
{
    if (hframeBufferObject && image.width() == hframeBufferObject->width() && himage.height() == hframeBufferObject->height()) {
        makeCurrent();
        glBindTexture(GL_TEXTURE_2D, hframeBufferObject->texture());
        if (himage.colors() == 1){
            if (himage.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, (unsigned char *)himage.constPointer());
            } else if (himage.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_SHORT, (unsigned char *)himage.constPointer());
            } else if (himage.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, (unsigned char *)himage.constPointer());
            }
        } else if (himage.colors() == 2){
            if (himage.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_UNSIGNED_BYTE, (unsigned char *)himage.constPointer());
            } else if (himage.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_UNSIGNED_SHORT, (unsigned char *)himage.constPointer());
            } else if (himage.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, (unsigned char *)himage.constPointer());
            }
        } else if (himage.colors() == 3){
            if (himage.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)himage.constPointer());
            } else if (himage.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_SHORT, (unsigned char *)himage.constPointer());
            } else if (himage.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, (unsigned char *)himage.constPointer());
            }
        } else if (himage.colors() == 4){
            if (himage.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *)himage.constPointer());
            } else if (himage.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_SHORT, (unsigned char *)himage.constPointer());
            } else if (himage.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (unsigned char *)himage.constPointer());
            }
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEOpenGLHistWidget::initializeGL()
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
    hvertexArrayObject.create();
    hvertexArrayObject.bind();

    // CREATE A BUFFER TO HOLD THE ROW AND COLUMN COORDINATES OF IMAGE PIXELS FOR THE TEXEL FETCHES
    hquadVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    hquadVertexBuffer.create();
    hquadVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (hquadVertexBuffer.bind()) {
        hquadVertexBuffer.allocate(16 * sizeof(float));
        float *vertices = (float *)hquadVertexBuffer.map(QOpenGLBuffer::WriteOnly);
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

            hquadVertexBuffer.unmap();
        } else {
            qDebug() << QString("Unable to map quadVertexBuffer from GPU.");
        }
    }

    // CREATE AN INDEX BUFFER FOR THE INCOMING DEPTH VIDEO DRAWN AS POINTS
    hquadIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    hquadIndexBuffer.create();
    hquadIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (hquadIndexBuffer.bind()) {
        hquadIndexBuffer.allocate(6 * sizeof(unsigned int));
        unsigned int *indices = (unsigned int *)hquadIndexBuffer.map(QOpenGLBuffer::WriteOnly);
        if (indices) {
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 0;
            indices[4] = 2;
            indices[5] = 3;
            hquadIndexBuffer.unmap();
        } else {
            qDebug() << QString("quadIndexBuffer buffer mapped from GPU.");
        }
    }

    // CREATE GLSL PROGRAM FOR PROCESSING THE INCOMING VIDEO
    setlocale(LC_NUMERIC, "C");
    hfilter.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ggehistcalc.vert");
    hfilter.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/ggehistcalc.geom");
    hfilter.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ggehistcalc.frag");
    hfilter.link();

    hprogram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ggehistdisp.vert");
    hprogram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ggehistdisp.frag");
    hprogram.link();
    setlocale(LC_ALL, "");

    // UPLOAD IMAGE TO TEXTURE
    if (himage.isNull() == false) {
        onSetImage(himage);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEOpenGLHistWidget::resizeGL(int w, int h)
{
    // Get the Desktop Widget so that we can get information about multiple monitors connected to the system.
    QDesktopWidget *dkWidget = QApplication::desktop();
    QList<QScreen *> screenList = QGuiApplication::screens();
    qreal devicePixelRatio = screenList[dkWidget->screenNumber(this)]->devicePixelRatio();
    localHeight = h * devicePixelRatio;
    localWidth = w * devicePixelRatio;

    qDebug() << localWidth << localHeight << w << h;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEOpenGLHistWidget::paintGL()
{
    // CLEAR THE FRAME BUFFER OBJECT
    glViewport(0, 0, localWidth, localHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (htexture) {
        if (hprogram.bind()) {
            // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
            if (hquadVertexBuffer.bind()) {
                if (hquadIndexBuffer.bind()) {
                    // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, hframeBufferObject->texture());
                    hprogram.setUniformValue("qt_texture", 1);
                    hprogram.setUniformValue("qt_flip", false);

                    // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                    glVertexAttribPointer(hprogram.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                    hprogram.enableAttributeArray("qt_vertex");
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                    hquadIndexBuffer.release();
                }
                hquadVertexBuffer.release();
            }
            hprogram.release();
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
GGEImageWidget::GGEImageWidget(LAUMemoryObject image, QWidget *parent) : QWidget(parent), isValidFlag(false), label(NULL)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0, 0, 0, 0);

    if (image.isNull()) {
        QSettings settings;
        QString directory = settings.value(QString("GGEImageWidget::lastDirectory"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();;
        QString filename = QFileDialog::getOpenFileName(NULL, QString("Select image file to open..."), directory, QString("*.tiff *.tif *.jpg"));
        if (filename.isEmpty() == false) {
            QString string = QFileInfo(filename).absolutePath();
            settings.setValue(QString("GGEImageWidget::lastDirectory"), string);
            image = LAUMemoryObject(filename);
        }
    }

    if (image.isNull() == false) {
        aspectRatio = (float)image.width() / (float)image.height();
        label = new GGEOpenGLWidget(image);
        isValidFlag = true;
        this->layout()->addWidget(label);
        this->setMinimumSize(240 * aspectRatio, 240);
    }
}/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
GGEHistWidget::GGEHistWidget(LAUMemoryObject himage, QWidget *parent) : QWidget(parent), hisValidFlag(false), hlabel(NULL)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0, 0, 0, 0);

    if (himage.isNull()) {
        QSettings settings;
        QString directory = settings.value(QString("GGEImageWidget::lastDirectory"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();;
        QString filename = QFileDialog::getOpenFileName(NULL, QString("Select image file to open..."), directory, QString("*.tiff *.tif *.jpg"));
        if (filename.isEmpty() == false) {
            QString string = QFileInfo(filename).absolutePath();
            settings.setValue(QString("GGEImageWidget::lastDirectory"), string);
            himage = LAUMemoryObject(filename);
        }

    }

    if (himage.isNull() == false) {
        haspectRatio = (float)himage.width() / (float)himage.height();
        hlabel = new GGEOpenGLHistWidget(himage);
        hisValidFlag = true;
        this->layout()->addWidget(hlabel);
        this->setMinimumSize(240 * haspectRatio, 240);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
GGEImageWidget::~GGEImageWidget()
{
    ;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEImageWidget::resizeEvent(QResizeEvent *)
{
    int wdth = this->height() * aspectRatio;
    this->resize(wdth, this->height());
}/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
GGEHistWidget::~GGEHistWidget()
{
    ;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEHistWidget::resizeEvent(QResizeEvent *)
{
    int wdth = this->height() * haspectRatio;
    this->resize(wdth, this->height());
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void GGEImageDialog::resizeEvent(QResizeEvent *)
{
    if (widget) {
        this->resize(widget->width() + 2 * widget->geometry().left(), this->height());
    }
}
