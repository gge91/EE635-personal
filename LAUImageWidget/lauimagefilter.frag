#version 410 core

uniform sampler2D qt_texture; // USER SUPPLIED HANDLE FOR OUR TARGET TEXTURE
uniform bool qt_flip;
in vec2 qt_coordinate;        // WIDGET COORDINATE DERIVED FROM VERTEX SHADER

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main(void)
{
    if (qt_flip){
        qt_fragColor = texture(qt_texture, qt_coordinate, 0);
    } else {
        qt_fragColor = texture(qt_texture, vec2(qt_coordinate.x, 1.0 - qt_coordinate.y), 0);
    }
    qt_fragColor = 1.0 - qt_fragColor;
}
