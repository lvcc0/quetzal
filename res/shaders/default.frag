#version 330 core

#define NR_DIR_LIGHTS 2
#define NR_POINT_LIGHTS 2
#define NR_SPOT_LIGHTS 2

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 color;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quad;

    vec3 color;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quad;

    float innerCutoff;
    float outerCutoff;

    vec3 color;
};

uniform vec3 viewPos;

uniform Material material;

uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec3 GetDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 GetSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result;

    for (int i = 0; i < NR_DIR_LIGHTS; i++)
        result += GetDirLight(dirLights[i], norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += GetPointLight(pointLights[i], norm, FragPos, viewDir);

    for (int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += GetSpotLight(spotLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 GetDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoord)) * diff;
    vec3 specular = light.specular * vec3(texture(material.specular, TexCoord)) * spec;

    return (ambient + diffuse + specular) * light.color;
}

vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quad * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoord)) * diff;
    vec3 specular = light.specular * vec3(texture(material.specular, TexCoord)) * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * light.color;
}

vec3 GetSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quad * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoord)) * diff;
    vec3 specular = light.specular * vec3(texture(material.specular, TexCoord)) * spec;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular) * light.color;
}