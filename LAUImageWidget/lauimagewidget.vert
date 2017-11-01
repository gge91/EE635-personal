#version 410 core

uniform sampler2D qt_texture;
uniform ivec2 size;
in  vec2 qt_vertex;       // POINTS TO VERTICES PROVIDED BY USER ON CP
out vec4 qt_geometry; // OUTPUT COORDINATE TO GEOMETRY SHADER

void main(void)
{
    qt_geometry = texelFetch(qt_texture, ivec2(qt_vertex.xy), 0);
    //ivec2 coord;
    //qt_geometry = texture(tex,vec2(float(coord.x) / float(size.x), float(coord.y) / float(size.y)));
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0); //DO NOTHING
}
