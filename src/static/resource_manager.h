#pragma once

// std
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <filesystem>
#include <vector>
#include <map>
#include <algorithm>

#include "scene/resources/mesh.h"
#include "scene/resources/texture.h"
#include "scene/resources/shader.h"

#include "core/rendering/shader_program.h"

// TODO: (?) consider moving from strings to std::filesystem::path
// TODO: catching exceptions when the resource is not loaded
// TODO: load cubemap faces from one image
// TODO: rewrite most of the functions to take references (const& where possible)
// TODO: move resource loading to here, not their constructors

class ResourceManager
{
public:
    ResourceManager() = delete;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;

    // Load single resource

    static void loadMesh(const std::string& file_path);
    static void loadTexture(const std::string& file_path, const std::string& texture_type);
    static void loadShader(const std::string& file_path);

    // Return resource's sptr

    static std::shared_ptr<qtzl::Mesh>    getMesh(const std::string& file_path);
    static std::shared_ptr<qtzl::Texture> getTexture(const std::string& file_path);
    static std::shared_ptr<qtzl::Shader>  getShader(const std::string& file_path);

    // Return maps

    static std::map<std::string, std::shared_ptr<qtzl::Mesh>>    getMeshes();
    static std::map<std::string, std::shared_ptr<qtzl::Texture>> getTextures();
    static std::map<std::string, std::shared_ptr<qtzl::Shader>>  getShaders();

    // The stuff below is not really resources so they don't get preloaded upon preloadResources()
    // Shader program stuff, must be used after loading shaders!

    static std::shared_ptr<ShaderProgram> createShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path, ShaderProgram::Type type = ShaderProgram::Type::DEFAULT);

    static std::vector<std::shared_ptr<ShaderProgram>> createPPShaderPrograms();

    static std::shared_ptr<ShaderProgram> getShaderProgram(const std::string& name);
    static std::shared_ptr<ShaderProgram> getPPShaderProgram(const std::string& name);

    static std::vector<std::shared_ptr<ShaderProgram>> getShaderPrograms();
    static std::vector<std::shared_ptr<ShaderProgram>> getPPShaderPrograms();

    // Loads 6 first textures in a directory (right, left, top, bottom, back, front)
    static unsigned int loadCubemap(const std::string& dir_path);

    // Preload all the resources in the RES_PATH directory
    static void preloadResources();

    // True if the resource with the given name is found in any of the maps
    static bool isLoaded(const std::string& file_path);

    // Returns resource's type based on its extension
    static std::string getType(const std::string& file_path);

private:
    inline static const std::string MESH_FILE_EXTENSION = ".obj";
    inline static const std::vector<std::string> TEXTURE_FILE_EXTENSIONS = { ".jpg", ".png" };

    inline static const std::string VERTEX_SHADER_FILE_EXTENSION = ".vert";
    inline static const std::string FRAGMENT_SHADER_FILE_EXTENSION = ".frag";

    // Maps of loaded resources

    inline static std::map<std::string, std::shared_ptr<qtzl::Mesh>>    m_LoadedMeshes;   // "mesh_path", *Mesh
    inline static std::map<std::string, std::shared_ptr<qtzl::Texture>> m_LoadedTextures; // "texture_path", *Texture
    inline static std::map<std::string, std::shared_ptr<qtzl::Shader>>  m_LoadedShaders;  // "shader_path", *Shader
    
    // Not exactly resources but they fit here :)

    inline static std::vector<std::shared_ptr<ShaderProgram>> m_ShaderPrograms;
    inline static std::vector<std::shared_ptr<ShaderProgram>> m_PPShaderPrograms;

    inline static std::vector<unsigned int> m_Cubemaps;

    // Returns file contents as a string
    static std::string getFileString(const std::string& file_path);
};