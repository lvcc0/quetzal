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
#include <glm/gtx/string_cast.hpp>

#include "shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec3 Normal;

    // Constructor
    Vertex( glm::vec3 pos, glm::vec2 texCoord, glm::vec3 norm) :
        Position(pos), TexCoord(texCoord), Normal(norm) { /* empty */ }
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
    
    glm::mat4 m_model_matrix = glm::mat4(1.0f);

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    std::vector<Texture> m_textures;
    std::vector<Texture> m_textures_loaded;

    // Constructor
    Model(std::string const& path);
    
    // Draw model
    void Draw(Shader& shader);

    // Moving in world space
    void translate(glm::vec3 vector);
    void scale(glm::vec3 vector);
    void rotate(float degrees, glm::vec3 vector);

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