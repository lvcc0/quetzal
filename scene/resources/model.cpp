#include "model.h"

Model::Model(const std::string& path, bool is_preload, std::string name)
    : Scene_Node(name), Scene_Object()
{
    this->loadModel(path);
}

Model::Model(const Model& obj)
    : Scene_Node(obj), Scene_Object(obj)
{
}

void Model::draw(const Shaders_pack& shaders) 
{
    for (unsigned int i = 0; i < this->m_Meshes.size(); i++)
        this->m_Meshes[i].draw(*shaders.MAIN_SHADER);

    // TODO: drawing with outline
}

//void Model::setupRender()
//{
//    // Vertex Array Object
//    vao_ptr = std::make_unique<VAO>();
//
//    // Vertex Buffer Object
//    vbo_ptr = std::make_unique<VBO>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
//
//    // Element Array Buffer
//    ibo_ptr = std::make_unique<IBO>(&m_Indices[0], m_Indices.size());
//
//    VB_Vertex_Layout layout;
//    layout.push<GLfloat>(3, offsetof(Vertex, Position));
//    layout.push<GLfloat>(2, offsetof(Vertex, TexCoord));
//    layout.push<GLfloat>(3, offsetof(Vertex, Normal));
//
//    vao_ptr->addBuffer(*vbo_ptr, layout);
//}

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = directory + '/' + std::string(path);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, componentsNum;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &componentsNum, 0);

    if (data)
    {
        GLenum format;
        if (componentsNum == 1)
            format = GL_RED;
        else if (componentsNum == 3)
            format = GL_RGB;
        else if (componentsNum == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        std::cout << "Texture failed to load at path: " << path << std::endl;

    stbi_image_free(data);
    return textureID;
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    this->m_Directory = path.substr(0, path.find_last_of('\\'));
    this->processNode(scene->mRootNode, scene);
}

std::vector<Texture> Model::loadTextures(aiMaterial* material, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString string;
        material->GetTexture(type, i, &string);

        bool skip = false;

        for (unsigned int j = 0; j < this->m_Textures.size(); j++)
        {
            if (std::strcmp(this->m_Textures[j]->m_Path.data(), string.C_Str()) == 0)
            {
                textures.push_back(*this->m_Textures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture = Texture(string.C_Str(), typeName);

            textures.push_back(texture);
            this->m_Textures.push_back(std::make_shared<Texture>(texture));
        }
    }

    return textures;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->m_Meshes.push_back(this->processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        this->processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    glm::vec3 vec3; // placeholder
    glm::vec2 vec2; // stuff
    
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

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = this->loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = this->loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return Mesh(vertices, indices, textures);
}