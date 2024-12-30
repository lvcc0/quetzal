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

    // Load single object
    static void loadModel(const std::string& file_path);
    static void loadTexture(const std::string& file_path);
    static void loadShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    
    // Load multiple objects
    static void loadModels(const std::string& dir_path);
    static void loadTextures(const std::string& dir_path);
    static void loadShaders(const std::string& dir_path);

    // Make shared_ptrs
    static std::shared_ptr<Shader> makeShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

    // Make objects
    static CylindricalBillboard makeCylBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts);
    static SphericalBillboard   makeSphBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_name, std::vector<Vertex> verts);
    
    // Return shared_ptrs
    static std::shared_ptr<Texture> getTexture(const std::string& name); // TODO: return a copy of an object rather than a shared_ptr?

    // Return objects
    static Model getModel(const std::string& name);

    // Return maps
    inline static std::map<const std::string, std::shared_ptr<Shader>> getScreenShaders() { return m_LoadedScreenShaders; }
    inline static std::map<const std::string, Model> getModels() { return m_LoadedModels; }
    
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

    // Map of loaded objects (first - filename, second - object ptrs)
    inline static std::map<std::string, std::shared_ptr<void>> m_LoadedObjects;

    // Maps of loaded resources in ptrs (first - name, second - ptrs)
    inline static std::map<const std::string, std::shared_ptr<Shader>> m_LoadedShaders;
    inline static std::map<const std::string, std::shared_ptr<Shader>> m_LoadedScreenShaders;

    // Maps of loaded resources (first - name, second - objects)
    inline static std::map<const std::string, Model>   m_LoadedModels;
    inline static std::map<const std::string, Texture> m_LoadedTextures;

    // Returns file contents as a string
    static std::string getFileString(const std::string& file_path); 
};