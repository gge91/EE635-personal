#version 330 core

uniform sampler2D qt_textureA; // USER SUPPLIED HANDLE FOR OUR TARGET TEXTURE
uniform sampler2D qt_textureB; // USER SUPPLIED HANDLE FOR OUR TARGET TEXTURE

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main(void)
{
    ivec2 limits = textureSize(qt_textureB, 0);

    qt_fragColor = vec4(0.0, 0.0, 0.0, 0.0);
    for (int row = 0; row < limits.y; row++){
        for (int col = 0; col < limits.x; col++){
            float
        }
    }

    return;
}
