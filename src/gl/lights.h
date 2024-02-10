#ifndef LIGHTS_H
#define LIGHTS_H

#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.h"

class DirLight
{
public:
	bool Enabled;
	const char* Name;

	glm::vec3 Direction;

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	glm::vec3 Color;

	DirLight(
		bool enabled,
		const char* name,
		glm::vec3 dir,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		glm::vec3 color
	);

	void UpdateUni(Shader shader, int index);

	operator const char* () { return Name; }
};

class PointLight
{
public:
	bool Enabled;
	const char* Name;

	glm::vec3 Position;

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float Constant;
	float Linear;
	float Quad;

	glm::vec3 Color;

	PointLight(
		bool enabled,
		const char* name,
		glm::vec3 pos,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float constant,
		float linear,
		float quad,
		glm::vec3 color
	);

	void UpdateUni(Shader shader, int index);

	operator const char* () { return Name; }
};

class SpotLight
{
public:
	bool Enabled;
	const char* Name;

	glm::vec3 Position;
	glm::vec3 Direction;

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float Constant;
	float Linear;
	float Quad;

	float InnerCutoff;
	float OuterCutoff;

	glm::vec3 Color;

	SpotLight(
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
		glm::vec3 color
	);

	void UpdateUni(Shader shader, int index);

	operator const char* () { return Name; }
};

#endif