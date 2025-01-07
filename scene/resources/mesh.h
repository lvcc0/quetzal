//#pragma once

//#include <vector>
//#include <string>
//
//#include <glad/glad.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include "vertex.h"
//#include "texture.h"
//#include "shader.h"
//
//class Mesh
//{
//public:
//	std::vector<Vertex> m_Vertices;
//	std::vector<unsigned int> m_Indices;
//	std::vector<Texture> m_Textures;
//
//	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
//
//	void draw(Shader& shader);
//
//private:
//	unsigned int VAO, VBO, EBO;
//
//	void setupMesh();
//};

#pragma once

#include "scene/main/resource.h"

namespace qtzl
{
	class Mesh : public Resource
	{
		// TODO
	};
}