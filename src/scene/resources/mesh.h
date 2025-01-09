#pragma once

// std
#include <vector>

// thirdparty
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "scene/main/resource.h"
#include "scene/resources/texture.h"

#include "core/rendering/vertex.h"

namespace qtzl
{
    // Resource that contains vertex array based geometry
    class Mesh : public Resource
    {
    public:
        Mesh(const std::string& name, const std::string& path);
        virtual ~Mesh() = default;

    private:
        unsigned int VAO, VBO, EBO;

        std::vector<Vertex> m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<Texture> m_Textures;

        std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType type, std::string type_name);
    };
}