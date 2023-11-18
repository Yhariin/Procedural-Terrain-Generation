#version 330 core 

in vec3 color;
in vec3 newNorm;

out vec4 fragColor;

uniform vec3 light_direction;

struct Light
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


void main()
{
    
    vec3 color_diffuse = color * clamp(dot(newNorm, normalize(light_direction)), 0, 1);

    // fragColor = vec4(color_diffuse, 1.0);
    fragColor = vec4(newNorm, 1.0);
    // fragColor = vec4(MVP[0][3], MVP[1][3], MVP[2][3], 1.0);
}