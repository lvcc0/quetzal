#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "texture.h"
#include "mesh.h"
#include "base_classes.h"

class Model // : public Renderable, public Scene_Node, public Scene_Object
{
public:
    float m_StencilScaling = 1.4f;

    bool is_selected = false;

    std::vector<std::shared_ptr<Texture>> m_Textures;
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;

    // Constructor
    Model(const std::string& path, bool is_preload = false, std::string name = "");
    Model(const Model& obj);

    // Draw model
    void draw(const Shaders_pack& shaders);

    std::string getName() { return "cool name"; }
    
    glm::vec3 m_Position = glm::vec3(0.0f);
    glm::vec3 m_Scale = glm::vec3(0.0f);
    glm::vec3 m_RotationDegrees = glm::vec3(0.0f);

    void setParent(const std::shared_ptr<Scene_Node>& parent) { std::cout << "parent" << std::endl; }
    void setName(std::string name) { std::cout << "name" << std::endl; }

    //glm::mat4 getModelMatrix() const noexcept;
protected:
    void loadModel(const std::string& path);
    std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType type, std::string typeName);
    
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    //// Moving in world space
    //inline virtual void setPosition() override;
    //inline virtual void setScale() override;
    //inline virtual void setRotationDegrees(const glm::vec3 rotation) override;

    //// Setup VAO, VBO, EBO
    //void setupRender() override;
};