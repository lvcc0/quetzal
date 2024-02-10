#include "lights.h"

DirLight::DirLight(
	bool enabled,
	const char* name,
	glm::vec3 dir,
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	glm::vec3 color)
{
	Enabled = enabled;
	Name = name;
	Direction = dir;
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
	Color = color;
}

void DirLight::UpdateUni(Shader shader, int index)
{
	std::string name = "dirLights[" + std::to_string(index) + "]";

    shader.setVec3(name + ".direction", Direction);

    shader.setVec3(name + ".ambient", Ambient);
    shader.setVec3(name + ".diffuse", Diffuse);
    shader.setVec3(name + ".specular", Specular);

    shader.setVec3(name + ".color", Enabled ? Color : glm::vec3(0.0f));
}

PointLight::PointLight(
	bool enabled,
	const char* name,
	glm::vec3 pos,
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	float constant,
	float linear,
	float quad,
	glm::vec3 color)
{
	Enabled = enabled;
	Name = name;
	Position = pos;
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
	Constant = constant;
	Linear = linear;
	Quad = quad;
	Color = color;
}

void PointLight::UpdateUni(Shader shader, int index)
{
	std::string name = "pointLights[" + std::to_string(index) + "]";

	shader.setVec3(name + ".position", Position);

	shader.setVec3(name + ".ambient", Ambient);
	shader.setVec3(name + ".diffuse", Diffuse);
	shader.setVec3(name + ".specular", Specular);

	shader.setFloat(name + ".constant", Constant);
	shader.setFloat(name + ".linear", Linear);
	shader.setFloat(name + ".quad", Quad);

	shader.setVec3(name + ".color", Enabled ? Color : glm::vec3(0.0f));
}

SpotLight::SpotLight(
	bool enabled,
	const char* name,
	glm::vec3 pos,
	glm::vec3 dir,
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	float constant,
	float linear,
	float quad,
	float innerCutoff,
	float outerCutoff,
	glm::vec3 color)
{
	Enabled = enabled;
	Name = name;
	Position = pos;
	Direction = dir;
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
	Constant = constant;
	Linear = linear;
	Quad = quad;
	InnerCutoff = innerCutoff;
	OuterCutoff = outerCutoff;
	Color = color;
}

void SpotLight::UpdateUni(Shader shader, int index)
{
	std::string name = "spotLights[" + std::to_string(index) + "]";

	shader.setVec3(name + ".position", Position);
	shader.setVec3(name + ".direction", Direction);

	shader.setVec3(name + ".ambient", Ambient);
	shader.setVec3(name + ".diffuse", Diffuse);
	shader.setVec3(name + ".specular", Specular);

	shader.setFloat(name + ".constant", Constant);
	shader.setFloat(name + ".linear", Linear);
	shader.setFloat(name + ".quad", Quad);
	
	shader.setFloat(name + ".innerCutoff", glm::cos(glm::radians(InnerCutoff)));
	shader.setFloat(name + ".outerCutoff", glm::cos(glm::radians(OuterCutoff)));
	
	shader.setVec3(name + ".color", Enabled ? Color : glm::vec3(0.0f));
}