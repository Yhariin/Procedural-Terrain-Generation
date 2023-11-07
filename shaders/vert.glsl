#version 330 core

in vec3 vPos;
in vec3 vCol;
out vec3 color;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    // gl_Position = vec4(vPos, 1.0);
    color = vCol;
}