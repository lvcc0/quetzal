#include "scene/resources/mesh.h"

namespace qtzl
{
    SubMesh::SubMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        : m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
    {
    }

    std::vector<Vertex> SubMesh::getVertices() const
    {
        return this->m_Vertices;
    }

    std::vector<unsigned int> SubMesh::getIndices() const
    {
        return this->m_Indices;
    }

    std::vector<Texture> SubMesh::getTextures() const
    {
        return this->m_Textures;
    }

    void SubMesh::bindVAO()
    {
        this->m_VAO_uptr->bind();
    }

    void SubMesh::unbindVAO()
    {
        this->m_VAO_uptr->unbind();
    }

    void SubMesh::setup()
    {
        this->m_VAO_uptr = std::make_unique<VAO>();
        this->m_VBO_uptr = std::make_unique<VBO>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
        this->m_EBO_uptr = std::make_unique<EBO>(&m_Indices[0], m_Indices.size());

        VertexBufferLayout layout;
        layout.push(3, offsetof(Vertex, Position));
        layout.push(2, offsetof(Vertex, TexCoord));
        layout.push(3, offsetof(Vertex, Normal));

        this->m_VAO_uptr->addBuffer(*this->m_VBO_uptr, layout);
    }

    // --- //

    Mesh::Mesh(const std::string& name, const std::string& path)
        : Resource(name, path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_OptimizeGraph
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        this->processNode(scene->mRootNode, scene);
    }

    std::vector<std::shared_ptr<SubMesh>> Mesh::getSubMeshes() const
    {
        return this->m_SubMeshes;
    }

    void Mesh::processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_SubMeshes.push_back(processSubMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    std::shared_ptr<SubMesh> Mesh::processSubMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // reserve some space for all the data
        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumFaces * 3);
        
        glm::vec3 vec3; // vector for storing temporal data
        glm::vec2 vec2; // vector for storing temporal data
        
        // Vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
        
            // position
            vec3.x = mesh->mVertices[i].x;
            vec3.y = mesh->mVertices[i].y;
            vec3.z = mesh->mVertices[i].z;
            vertex.Position = vec3;
        
            // normals
            if (mesh->HasNormals())
            {
                vec3.x = mesh->mNormals[i].x;
                vec3.y = mesh->mNormals[i].y;
                vec3.z = mesh->mNormals[i].z;
                vertex.Normal = vec3;
            }
        
            // texture coordinates
            if (mesh->mTextureCoords[0])
            {
                vec2.x = mesh->mTextureCoords[0][i].x;
                vec2.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoord = vec2;
            }
            else
                vertex.TexCoord = glm::vec2(0.0f);
        
            vertices.push_back(vertex);
        }
        
        // Indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        
        // Textures
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<Texture> diffuseMaps = this->loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        std::vector<Texture> specularMaps = this->loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
        
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        return std::make_shared<SubMesh>(vertices, indices, textures);
    }

    std::vector<Texture> Mesh::loadTextures(aiMaterial* material, aiTextureType type, std::string type_name)
    {
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
        {
            aiString path; // relative to the .obj file
            material->GetTexture(type, i, &path);

            bool skip = false;

            for (unsigned int j = 0; j < this->m_LoadedTextures.size(); j++)
            {
                if (std::strcmp(this->m_LoadedTextures[j].getPath().data(), path.C_Str()) == 0)
                {
                    textures.push_back(this->m_LoadedTextures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                // TODO: load this texture with the resource manager
                Texture texture = Texture(path.C_Str(), std::filesystem::path(m_Path).parent_path().string() + "/" + path.C_Str(), type_name);

                textures.push_back(texture);
                this->m_LoadedTextures.push_back(texture);
            }
        }

        return textures;
    }
}