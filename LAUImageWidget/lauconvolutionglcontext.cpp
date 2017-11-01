#include "lauconvolutionglcontext.h"


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUConvolutionGLContext::initialize()
{
    if (makeCurrent(surface)){
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

        // CREATE A BUFFER TO HOLD THE ROW AND COLUMN COORDINATES OF obj PIXELS FOR THE TEXEL FETCHES
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
        program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lauobjwidget.vert");
        program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/lauobjwidget.frag");
        program.link();
        setlocale(LC_ALL, "");

        // UPLOAD OBJECTS TO TEXTURES
        textureA = setTexture(objectA);
        textureB = setTexture(objectB);

        // CREATE OUR FRAME BUFFER OBJECT TO RECIEVE OUR OUTPUT IMAGE
        QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
        frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);
        frameBufferObject = new QOpenGLFramebufferObject(image.size(), frameBufferObjectFormat);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUConvolutionGLContext::filter()
{
    if (makeCurrent(surface) && textureA && textureB){
        ;
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************"************************************************/
void LAUConvolutionGLContext::result(LAUMemoryObject object)
{
    if (frameBufferObject && object.width() == frameBufferObject->width() && object.height() == frameBufferObject->height()) {
        // COPY RESULT TO CPU
        glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
        if (object.colors() == 1){
            if (object.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, (unsigned char *)object.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_SHORT, (unsigned char *)object.constPointer());
            } else if (image.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, (unsigned char *)object.constPointer());
            }
        } else if (object.colors() == 2){
            if (object.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_UNSIGNED_BYTE, (unsigned char *)object.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_UNSIGNED_SHORT, (unsigned char *)object.constPointer());
            } else if (image.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RG, GL_FLOAT, (unsigned char *)object.constPointer());
            }
        } else if (object.colors() == 3){
            if (object.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)object.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_SHORT, (unsigned char *)object.constPointer());
            } else if (image.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, (unsigned char *)object.constPointer());
            }
        } else if (object.colors() == 4){
            if (object.depth() == sizeof(unsigned char)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *)object.constPointer());
            } else if (image.depth() == sizeof(unsigned short)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_SHORT, (unsigned char *)object.constPointer());
            } else if (image.depth() == sizeof(float)){
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (unsigned char *)object.constPointer());
            }
        }
    }
    return;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
QOpenGLTexture LAUConvolutionGLContext::setTexture(LAUMemoryObject obj)
{
    QOpenGLTexture texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setSize(obj.width(), obj.height());
    texture->setFormat(QOpenGLTexture::RGBA32F);
    texture->setWrapMode(QOpenGLTexture::ClampToBorder);
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    texture->allocateStorage();
    if (obj.colors() == 1){
        if (obj.depth() == sizeof(unsigned char)){
            texture->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, (const void *)obj.constPointer());
        } else if (obj.depth() == sizeof(unsigned short)){
            texture->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt16, (const void *)obj.constPointer());
        } else if (obj.depth() == sizeof(float)){
            texture->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, (const void *)obj.constPointer());
        }
    } else if (obj.colors() == 2){
        if (obj.depth() == sizeof(unsigned char)){
            texture->setData(QOpenGLTexture::RG, QOpenGLTexture::UInt8, (const void *)obj.constPointer());
        } else if (obj.depth() == sizeof(unsigned short)){
            texture->setData(QOpenGLTexture::RG, QOpenGLTexture::UInt16, (const void *)obj.constPointer());
        } else if (obj.depth() == sizeof(float)){
            texture->setData(QOpenGLTexture::RG, QOpenGLTexture::Float32, (const void *)obj.constPointer());
        }
    } else if (obj.colors() == 3){
        if (obj.depth() == sizeof(unsigned char)){
            texture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)obj.constPointer());
        } else if (obj.depth() == sizeof(unsigned short)){
            texture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt16, (const void *)obj.constPointer());
        } else if (obj.depth() == sizeof(float)){
            texture->setData(QOpenGLTexture::RGB, QOpenGLTexture::Float32, (const void *)obj.constPointer());
        }
    } else if (obj.colors() == 4){
        if (obj.depth() == sizeof(unsigned char)){
            texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (const void *)obj.constPointer());
        } else if (obj.depth() == sizeof(unsigned short)){
            texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt16, (const void *)obj.constPointer());
        } else if (obj.depth() == sizeof(float)){
            texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::Float32, (const void *)obj.constPointer());
        }
    }
    return(texture);
}
