#pragma once

#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <iostream>
#include <vector>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include "billboards.h"
#include "model.h"

class ResourceManager
{
public:
    // Delete all constructors
    ResourceManager() = delete;
    ResourceManager(const ResourceManager& obj) = delete;
    ResourceManager(ResourceManager&& obj) = delete;

    // Load objects
    static void loadModel(const std::string model_path);
    static void loadTexture(const std::string texture_path);
    static void loadScreenShaders(const std::string path_to_folder);

    // Return processing shaders
    inline static std::map<const std::string, std::shared_ptr<Shader>> makePostProcessingShaders() { return m_LoadedScreenShaders; }

    // Return maps
    inline static std::map<const std::string, Model> takeModels() { return m_LoadedModels; }

    // Make shared_ptrs
    static std::shared_ptr<Shader>               makeShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    // In my opinion it should return a copy of an object
    static std::shared_ptr<Texture>              makeTexture(const std::string& name);

    // Make objects
    static Model                makeModel(const std::string& name);
    static CylindricalBillboard makeCylBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts);
    static SphericalBillboard   makeSphBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts);
    
    static void displayLoadedObjects();
    static void preLoadResources();

private:
    inline static const std::string relResPath = RES_PATH; // full path to res directory
    inline static const std::string MODELS_FOLDER = "objects";
    inline static const std::string SHADERS_FOLDER = "shaders";
    inline static const std::string TEXTURES_FOLDER = "textures";
    inline static const std::string SCREEN_SHADERS_FOLDER = "postprocess";

    inline static const std::string MODELS_MAIN_FILE_EXTENSION = ".obj";
    inline static const std::string VERTEX_FILE_EXTENSION = ".vert";
    inline static const std::string FRAGMENT_FILE_EXTENSION = ".frag";

    inline static const std::vector<std::string> TEXTURES_FILE_EXTENSIONS{ ".jpg", ".png" };

    // Map of loaded resources (First - name of file, second - ptrs of objects)
    inline static std::map<std::string, std::shared_ptr<void>> m_LoadedObjects;

    // Maps of loaded resources in ptrs (First - name, second - ptrs)
    inline static std::map<const std::string, std::shared_ptr<Shader>> m_LoadedShaders;
    inline static std::map<const std::string, std::shared_ptr<Shader>> m_LoadedScreenShaders;

    // Maps of loaded resources (First - name, second - objects)
    inline static std::map<const std::string, Model> m_LoadedModels;
    inline static std::map<const std::string, Texture> m_LoadedTextures;

    static std::string getFileString(const std::string& file_path); // gets file contents as a string

    static std::vector<std::shared_ptr<Texture>> pullTexturesFromMtl(const std::string& fullfilepath);
};