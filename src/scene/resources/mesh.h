#pragma once

// std
#include <filesystem>

// thirdparty
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "scene/main/resource.h"
#include "scene/resources/texture.h"

#include "core/rendering/vertex_array.h"
#include "core/rendering/shader_program.h"

namespace qtzl
{
    // Not exactly a resource, but an object used by the Mesh resource
    // Contains all the buffers necessary to render
    class SubMesh
    {
    public:
        SubMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        virtual ~SubMesh() = default;

        std::vector<Vertex> getVertices() const;
        std::vector<unsigned int> getIndices() const;
        std::vector<Texture> getTextures() const;

        void bindVAO();
        void unbindVAO();

        // Setup all the buffers (use this in the setupRender())
        void setup();

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<Texture> m_Textures;
    
        std::unique_ptr<VAO> m_VAO_uptr = nullptr;
        std::unique_ptr<VBO> m_VBO_uptr = nullptr;
        std::unique_ptr<EBO> m_EBO_uptr = nullptr;
    };

    // Resource that contains vertex array based geometry
    class Mesh : public Resource
    {
    public:
        Mesh(const std::string& name, const std::string& path);
        virtual ~Mesh() = default;

        std::vector<std::shared_ptr<SubMesh>> getSubMeshes() const;

    private:
        std::vector<std::shared_ptr<SubMesh>> m_SubMeshes;
        std::vector<Texture> m_LoadedTextures;

        // Get all the meshes in the given node and save them in the m_SubMeshes vector
        void processNode(aiNode* node, const aiScene* scene);

        // Get all the necessary data (vertiecs, indices, textures) from the aiMesh and return it as the SubMesh object
        std::shared_ptr<SubMesh> processSubMesh(aiMesh* mesh, const aiScene* scene);

        // Get all textures from given material
        std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType type, std::string type_name);
    };
}