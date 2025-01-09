#include "scene/resources/mesh.h"

namespace qtzl
{
    Mesh::Mesh(const std::string& name, const std::string& path)
        : Resource(name, path)
    {
        // Setup mesh data

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        aiMesh* mesh = scene->mMeshes[scene->mRootNode->mMeshes[0]];

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

        // Setup buffers

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->m_Vertices.size() * sizeof(Vertex), &this->m_Vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_Indices.size() * sizeof(unsigned int), &this->m_Indices[0], GL_STATIC_DRAW);

        // positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

        glBindVertexArray(0);
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
                Texture texture = Texture(path.C_Str(), path.C_Str(), type_name); // TODO: change texture name here

                textures.push_back(texture);
                this->m_Textures.push_back(texture);
            }
        }

        return textures;
    }
}