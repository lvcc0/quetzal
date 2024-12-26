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
#include "collision.h"

// model is a collection of meshes, so by itself it's not renderable, its meshes are
class Model : public Scene_Node, public Scene_Object
{
public:
    float m_StencilScaling = 1.4f;

    std::vector<std::shared_ptr<Texture>> m_Textures;
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;

    // Constructor
    Model(const std::string& path, bool is_preload = false, std::string name = "");
    Model(const Model& obj);

    // Draw model
    void draw(const Shaders_pack& shaders);

protected:
    void loadModel(const std::string& path);
    std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType type, std::string typeName);
    
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // TODO: moving, scaling, rotating models
    //// Moving in world space
    //inline virtual void setPosition() override;
    //inline virtual void setScale() override;
    //inline virtual void setRotationDegrees(const glm::vec3 rotation) override;

    //// Setup VAO, VBO, EBO
    //void setupRender() override;
};