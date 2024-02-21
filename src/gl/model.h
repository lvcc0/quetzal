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

#include "shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec3 Normal;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

unsigned int TextureFromFile(const char* path, const std::string& directory);

class Model
{
public:
    std::vector<Texture> textures_loaded;
    std::string directory;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Constructor
    Model(std::string const& path);

    void Draw(Shader& shader);

private:
    unsigned int VAO, VBO, EBO;

    void loadModel(std::string const& path);
    void loadMaterial(std::string const& path);
    void setupModel();
};

#endif