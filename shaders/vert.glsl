#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;
layout (location = 2) in vec3 vNorm;

out vec3 color;
out vec3 norm;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;


void main()
{
    vec3 fragPos = vec3(model * vec4(vPos, 1.0));
    norm = mat3(transpose(inverse(model))) * vNorm;

    fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
    
    gl_Position = projection * view * model * vec4(vPos, 1.0);

    color = vCol;
}