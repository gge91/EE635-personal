#version 410 core

layout (points) in;
layout (points, max_vertices = 4) out;

in vec4 qt_geometry[]; //point coord input from vertex

void main(void)
{
    gl_Position.x = 2.0*qt_geometry[0].r - 1.0 + 0.5/255.0;
    gl_Position.y = -0.75;
    gl_Position.z = 0.00;
    gl_Position.a = 1.00;
    EmitVertex();
    EndPrimitive();

    gl_Position.x = 2.0*qt_geometry[0].g - 1.0 + 0.5/255.0;
    gl_Position.y = -0.25;
    gl_Position.z = 0.00;
    gl_Position.a = 1.00;
    EmitVertex();
    EndPrimitive();

    gl_Position.x = 2.0*qt_geometry[0].b - 1.0 + 0.5/255.0;
    gl_Position.y = 0.25;
    gl_Position.z = 0.00;
    gl_Position.a = 1.00;
    EmitVertex();
    EndPrimitive();

    gl_Position.x = 2.0*qt_geometry[0].a - 1.0 + 0.5/255.0;
    gl_Position.y = 0.75;
    gl_Position.z = 0.00;
    gl_Position.a = 1.00;
    EmitVertex();
    EndPrimitive();
}
