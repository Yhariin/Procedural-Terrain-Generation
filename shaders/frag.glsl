#version 330 core 

in vec3 color;
in vec3 norm;
in vec4 fragPosLightSpace;

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
    vec3 lightDirection = normalize(-light_direction);
    
    vec3 color_diffuse = color * clamp(dot(norm, normalize(lightDirection)), 0, 1);


    fragColor = vec4(color_diffuse, 1.0);
    // fragColor = vec4(norm, 1.0);
}