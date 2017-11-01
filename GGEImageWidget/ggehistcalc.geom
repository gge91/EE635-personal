#version 330 core

layout (points) in;
layout (points, max_vertices = 4) out;

in vec2 qt_coordinate; //point coord input from vertex
out vec2 hist_coord;  //position of point to draw in for frag shader

void main(void)
{
    gl_Position = qt_coordinate;

    hist_coord = (gl_Position.r*256, 0.75);
    EndPrimitive();

    hist_coord = (gl_Position.g*256, 0.25);
    EndPrimitive();

    hist_coord = (gl_Position.b*256, -0.25);
    EndPrimitive();

    hist_coord = (gl_Position.a*256, -0.75);
    EndPrimitive();
}
