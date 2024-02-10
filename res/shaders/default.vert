#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 inversed;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    Normal = mat3(transpose(inversed)) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTex;
}