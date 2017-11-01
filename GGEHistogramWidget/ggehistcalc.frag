#version 330 core

uniform sampler2D qt_texture; // USER SUPPLIED HANDLE FOR OUR TARGET TEXTURE
uniform bool qt_flip;
in vec2 qt_coordinate;        // WIDGET COORDINATE DERIVED FROM VERTEX SHADER

void main(void)
{



            //need glEnable(GL_BLEND) turned on for alpha blending
            //glBlendFunc(GL_ONE,GL_ONE)
            //glBlendEquation(GL_FUNC_ADD)
            //glPointSize(1.0f) sets point size (1.0f is 1 point = 1pix)

            //in frag shader, need to set gl_position.x .y .z etc or else it won't work


    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}
