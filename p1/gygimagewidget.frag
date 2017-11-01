uniform sampler2D qt_texture; //user supplied handle for target texture
varying vec4 qt_TexCoord0;

layout(location = 0, index = 0) out vec4 qt_fragColor
// indicates texture is on screen

void main(void)
{
    gl_FragColor = texture(qt_texture, qt_TexCoord0, 0);
}
