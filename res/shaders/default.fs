#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D tex0;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform float ambientStr;
uniform float specularStr;

uniform vec3 viewPos;

void main()
{
    // ambient
    vec3 ambient = ambientStr * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStr * spec * lightColor;

    FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(tex0, TexCoord);
}