#include "model.h"
#include <glm/gtx/string_cast.hpp>

Model::Model(std::string const& path)
{
	loadModel(path);
	setupModel();
}

void Model::Draw(Shader& shader)
{
	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNum++);
		else if (name == "texture_specular")
			number = std::to_string(specularNum++);

		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Model::setupModel()
{
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Vertex Array Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Element Array Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
	// vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glBindVertexArray(0);
}

void Model::loadModel(std::string const& path)
{
	directory = path.substr(0, path.find_last_of('/'));

	std::ifstream file(path);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	std::vector<std::string> triplets;
	std::string line;

	while (std::getline(file, line))
	{
		std::string linetype = line.substr(0, 2);

		if (linetype ==  "# ") // comment
			continue;

		if (linetype == "mt") // mtllib (material file)
		{
			std::istringstream v(line.substr(7));
			loadMaterial(v.str());
		}

		if (linetype == "o ") // object (mesh)
		{
			// think something
		}

		if (linetype == "v ") // vertex
		{
			std::istringstream v(line.substr(2));
			glm::vec3 vector;

			v >> vector.x;
			v >> vector.y;
			v >> vector.z;

			positions.push_back(vector);
		}

		if (linetype == "vt") // texture
		{
			std::istringstream v(line.substr(2));
			glm::vec2 vector;

			v >> vector.x;
			v >> vector.y;

			texCoords.push_back(vector);
		}

		if (linetype == "vn") // normal
		{
			std::istringstream v(line.substr(2));
			glm::vec3 vector;

			v >> vector.x;
			v >> vector.y;
			v >> vector.z;

			normals.push_back(vector);
		}

		if (linetype == "f ") // face
		{
			std::istringstream v(line.substr(2));

			std::string points[3], token;
			
			std::vector<int> tokens;
			Vertex vertex;

			v >> points[0];
			v >> points[1];
			v >> points[2];

			for (unsigned int i = 0; i < 3; i++)
			{
				std::stringstream ss(points[i]);

				triplets.push_back(ss.str());
				indices.push_back(std::find(triplets.begin(), triplets.end(), ss.str()) - triplets.begin());

				while (getline(ss, token, '/'))
					tokens.push_back(std::stoi(token));

				vertex.Position = positions[tokens[0] - 1];
				vertex.TexCoord = texCoords[tokens[1] - 1];
				vertex.Normal = normals[tokens[2] - 1];

				vertices.push_back(vertex);
				tokens.clear();

				std::cout << indices[indices.size() - 1] << " ";
			}
			
			std::cout << "\n";
		}
	}
	
	/*
	for (Vertex const i : vertices)
		std::cout << "Vertices: " << glm::to_string(i.Position) << glm::to_string(i.TexCoord) << glm::to_string(i.Normal) << std::endl;
	*/
}

void Model::loadMaterial(std::string const& path)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	std::ifstream file(filename);
	std::string line;

	while (std::getline(file, line))
	{
		std::string linetype = line.substr(0, 6);

		if (linetype == "map_Kd")
		{
			std::istringstream v(line.substr(7));

			std::string str = v.str();
			bool skip = false;
			
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.c_str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				Texture texture;
				texture.id = TextureFromFile(str.c_str(), directory);
				texture.type = "texture_diffusal";
				texture.path = str.c_str();

				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
	}
}

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, numComponents;
	unsigned char* bytes = stbi_load(filename.c_str(), &width, &height, &numComponents, 0);

	if (bytes)
	{
		GLenum format;

		if (numComponents == 1)
			format = GL_RED;
		else if (numComponents == 3)
			format = GL_RGB;
		else if (numComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bytes);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Texture failed to load at: " << path << std::endl;

	stbi_image_free(bytes);
	return textureID;
}