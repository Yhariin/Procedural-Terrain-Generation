#version 330 core 

in vec3 color;
out vec4 fragColor;

// uniform mat4 MVP;
// uniform float test

void main()
{
    fragColor = vec4(color, 1.0);
    // fragColor = vec4(MVP[0][3], MVP[1][3], MVP[2][3], 1.0);
}