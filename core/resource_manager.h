#pragma once

// std
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <filesystem>
#include <vector>
#include <map>

#include "scene/resources/mesh.h"
#include "scene/resources/texture.h"
#include "scene/resources/shader.h"

#include "core/rendering/shader_program.h"

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

    static std::shared_ptr<qtzl::Mesh>    getMesh(const std::string& name) const;
    static std::shared_ptr<qtzl::Texture> getTexture(const std::string& name) const;
    static std::shared_ptr<qtzl::Shader>  getShader(const std::string& name) const;

    // Return maps

    static std::map<const std::string, std::shared_ptr<qtzl::Mesh>>    getMeshes() const;
    static std::map<const std::string, std::shared_ptr<qtzl::Texture>> getTextures() const;
    static std::map<const std::string, std::shared_ptr<qtzl::Shader>>  getShaders() const;

    // Return binded shader programs, must be used after loading shaders!

    static std::shared_ptr<ShaderProgram> getShaderProgram(const std::string& name) const;
    static std::shared_ptr<ShaderProgram> getPPShaderProgram(const std::string& name) const;

    static std::vector<std::shared_ptr<ShaderProgram>> getShaderPrograms() const;
    static std::vector<std::shared_ptr<ShaderProgram>> getPPShaderPrograms() const;

    // Preload all the resources in the res directory
    static void preLoadResources();

private:
    inline static const std::string MESHES_FOLDER = "objects";
    inline static const std::string TEXTURES_FOLDER = "textures";
    inline static const std::string SHADERS_FOLDER = "shaders";
    inline static const std::string PPSHADERS_FOLDER = "postprocess";

    inline static const std::string MESH_MAIN_FILE_EXTENSION = ".obj";
    inline static const std::string VERTEX_SHADER_FILE_EXTENSION = ".vert";
    inline static const std::string FRAGMENT_SHADER_FILE_EXTENSION = ".frag";

    inline static const std::vector<std::string> TEXTURES_FILE_EXTENSIONS = { ".jpg", ".png" };

    // Maps of loaded resources
    // NOTE: do we really need maps now when every resource has its name in it?
    inline static std::map<const std::string, std::shared_ptr<qtzl::Mesh>>    m_LoadedMeshes;   // "mesh_name", *Mesh
    inline static std::map<const std::string, std::shared_ptr<qtzl::Texture>> m_LoadedTextures; // "texture_name", *Texture
    inline static std::map<const std::string, std::shared_ptr<qtzl::Shader>>  m_LoadedShaders;  // "shader_name", *Shader

    // Returns file contents as a string
    static std::string getFileString(const std::string& file_path);
};