#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

unsigned int TextureFromFile(const char* path, const std::string& directory);

class Model
{
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

	Model(std::string const& path);
	void Draw(Shader& shader);

private:
	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif