#version 330 core

out vec4 FragColor;

uniform vec3 objectColor; // basically light color

void main()
{
    FragColor = vec4(objectColor, 1.0);
}