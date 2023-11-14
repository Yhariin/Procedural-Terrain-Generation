#version 330 core 

in vec3 color;
out vec4 fragColor;

struct Light
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


void main()
{

    fragColor = vec4(color, 1.0);
    // fragColor = vec4(MVP[0][3], MVP[1][3], MVP[2][3], 1.0);
}