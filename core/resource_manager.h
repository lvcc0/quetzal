#pragma once

// std
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <filesystem>
#include <vector>
#include <map>

#include "scene/resources/mesh.h"
#include "scene/resources/texture.h"
#include "scene/resources/shader.h"

class ResourceManager
{
public:
    // Delete all constructors
    ResourceManager() = delete;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;

    // Load single resource
    static void loadMesh(const std::string& file_path);
    static void loadTexture(const std::string& file_path, const std::string& texture_type);
    static void loadShader(const std::string& file_path);

    // Load multiple resources
    static void loadMeshes(const std::string& dir_path);
    static void loadTextures(const std::string& dir_path);
    static void loadShaders(const std::string& dir_path);

    // Return resource
    static std::shared_ptr<qtzl::Mesh>    getMesh(const std::string& name);
    static std::shared_ptr<qtzl::Texture> getTexture(const std::string& name);
    static std::shared_ptr<qtzl::Shader>  getShader(const std::string& name);

    // Return maps
    inline static std::map<const std::string, std::shared_ptr<qtzl::Mesh>>    getMeshes() { return m_LoadedModels; }
    inline static std::map<const std::string, std::shared_ptr<qtzl::Texture>> getTextures() { return m_LoadedTextures; }
    inline static std::map<const std::string, std::shared_ptr<qtzl::Shader>>  getShaders() { return m_LoadedShaders; }

    static void preLoadResources();

private:
    inline static const std::string relResPath = RES_PATH; // full path to res directory
    inline static const std::string MODELS_FOLDER = "objects";
    inline static const std::string SHADERS_FOLDER = "shaders";
    inline static const std::string TEXTURES_FOLDER = "textures";
    inline static const std::string PPSHADERS_FOLDER = "postprocess";

    inline static const std::string MESH_MAIN_FILE_EXTENSION = ".obj";
    inline static const std::string VERTEX_FILE_EXTENSION = ".vert";
    inline static const std::string FRAGMENT_FILE_EXTENSION = ".frag";

    inline static const std::vector<std::string> TEXTURES_FILE_EXTENSIONS = { ".jpg", ".png" };

    // Maps of loaded resources
    inline static std::map<const std::string, std::shared_ptr<qtzl::Mesh>>    m_LoadedMeshes;    // "mesh_name", *Mesh
    inline static std::map<const std::string, std::shared_ptr<qtzl::Texture>> m_LoadedTextures;  // "texture_name", *Texture
    inline static std::map<const std::string, std::shared_ptr<qtzl::Shader>>  m_LoadedShaders;   // "shader_name", *Shader

    // Returns file contents as a string
    static std::string getFileString(const std::string& file_path);
};