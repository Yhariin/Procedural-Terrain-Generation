#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;
layout (location = 2) in vec3 vNorm;

out vec3 color;
out vec3 newNorm;

uniform mat4 MVP;

void main()
{
    newNorm = normalize(vNorm);
    gl_Position = MVP * vec4(vPos, 1.0);
    // gl_Position = vec4(vPos, 1.0);
    color = vCol;
}