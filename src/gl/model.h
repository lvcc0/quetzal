#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    std::string m_directory;
    
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    std::vector<Texture> m_textures;
    std::vector<Texture> m_textures_loaded;

    // Constructor
    Model(std::string const& path);

    // Draw model
    void Draw(Shader& shader);

private:
    unsigned int VAO, VBO, EBO;

    // Read obj file
    void loadModel(std::string const& path);

    // Read mtl file
    void loadMaterial(std::string const& path);
    
    // Setup VAO, VBO, EBO
    void setupModel();
};

#endif