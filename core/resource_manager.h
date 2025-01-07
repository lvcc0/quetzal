#pragma once

#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <iostream>
#include <vector>
#include <filesystem>

#include "model.h"
#include "texture.h"
#include "shader.h"

// TODO: rewrite it for new resource types

class ResourceManager
{
public:
    // Delete all constructors
    ResourceManager() = delete;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;

    // Load single resource
    static void loadModel(const std::string& file_path);
    static void loadTexture(const std::string& file_path);
    static void loadShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    static void loadPPShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

    // Load multiple resources
    static void loadModels(const std::string& dir_path);
    static void loadTextures(const std::string& dir_path);
    static void loadShaders(const std::string& dir_path);
    static void loadPPShaders(const std::string& dir_path);

    // Return resource
    static std::shared_ptr<Model>   getModel(const std::string& name);
    static std::shared_ptr<Texture> getTexture(const std::string& name);
    static std::shared_ptr<Shader>  getShader(const std::string& name);
    static std::shared_ptr<Shader>  getPPShader(const std::string& name);

    // Return maps
    inline static std::map<const std::string, std::shared_ptr<Model>>   getModels() { return m_LoadedModels; }
    inline static std::map<const std::string, std::shared_ptr<Texture>> getTextures() { return m_LoadedTextures; }
    inline static std::map<const std::string, std::shared_ptr<Shader>>  getShaders() { return m_LoadedShaders; }
    inline static std::map<const std::string, std::shared_ptr<Shader>>  getPPShaders() { return m_LoadedPPShaders; }

    static void preLoadResources();

private:
    inline static const std::string relResPath = RES_PATH; // full path to res directory
    inline static const std::string MODELS_FOLDER = "objects";
    inline static const std::string SHADERS_FOLDER = "shaders";
    inline static const std::string TEXTURES_FOLDER = "textures";
    inline static const std::string PPSHADERS_FOLDER = "postprocess";

    inline static const std::string MODELS_MAIN_FILE_EXTENSION = ".obj";
    inline static const std::string VERTEX_FILE_EXTENSION = ".vert";
    inline static const std::string FRAGMENT_FILE_EXTENSION = ".frag";

    inline static const std::vector<std::string> TEXTURES_FILE_EXTENSIONS = { ".jpg", ".png" };

    // Maps of loaded resources
    inline static std::map<const std::string, std::shared_ptr<Shader>>  m_LoadedShaders;   // "vertex_shader.vert||fragment_shader.frag", *Shader (shader program with those shaders attached)
    inline static std::map<const std::string, std::shared_ptr<Shader>>  m_LoadedPPShaders; // "fragment_shader.frag", *Shader (shader program with this frag shader and that single vert shader in the postprocess dir)
    inline static std::map<const std::string, std::shared_ptr<Model>>   m_LoadedModels;    // "model_name", *Model
    inline static std::map<const std::string, std::shared_ptr<Texture>> m_LoadedTextures;  // "texture_name", *Texture

    // Returns file contents as a string
    static std::string getFileString(const std::string& file_path);

    // Returns a binded shader program
    static Shader makeShaderProgram(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
};