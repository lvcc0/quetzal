#include "scene/resources/mesh.h"

namespace qtzl
{
    Mesh::Mesh(const std::string& name, const std::string& path)
        : Resource(name, path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        aiMesh* mesh = scene->mMeshes[0];

        // reserve some space for all the data
        this->m_Vertices.reserve(mesh->mNumVertices);
        this->m_Indices.reserve(mesh->mNumFaces * 3);

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

            this->m_Vertices.push_back(vertex);
        }

        // Indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                this->m_Indices.push_back(face.mIndices[j]);
        }

        // Textures
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = this->loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        std::vector<Texture> specularMaps = this->loadTextures(material, aiTextureType_SPECULAR, "texture_specular");

        this->m_Textures.insert(this->m_Textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        this->m_Textures.insert(this->m_Textures.end(), specularMaps.begin(), specularMaps.end());
    }

    std::vector<Vertex> Mesh::getVertices() const
    {
        return this->m_Vertices;
    }

    std::vector<unsigned int> Mesh::getIndices() const
    {
        return this->m_Indices;
    }

    std::vector<Texture> Mesh::getTextures() const
    {
        return this->m_Textures;
    }

    std::vector<Texture> Mesh::loadTextures(aiMaterial* material, aiTextureType type, std::string type_name)
    {
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
        {
            aiString path;
            material->GetTexture(type, i, &path);

            bool skip = false;

            for (unsigned int j = 0; j < this->m_Textures.size(); j++)
            {
                if (std::strcmp(this->m_Textures[j].getPath().data(), path.C_Str()) == 0)
                {
                    textures.push_back(this->m_Textures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                Texture texture = Texture(path.C_Str(), std::filesystem::path(m_Path).parent_path().string() + "/" + path.C_Str(), type_name); // TODO: change texture name here

                textures.push_back(texture);
                this->m_Textures.push_back(texture);
            }
        }

        return textures;
    }
}