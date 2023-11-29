#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;
layout (location = 2) in vec3 vNorm;

out vec3 color;
out vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    norm = mat3(transpose(inverse(model))) * vNorm;
    
    gl_Position = projection * view * model * vec4(vPos, 1.0);

    color = vCol;
}