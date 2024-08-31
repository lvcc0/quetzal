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
#include "rigid_body.h"

class ResourceManager
{
public:

    // Delete all constructors
    ResourceManager() = delete;
    ResourceManager(const ResourceManager& obj) = delete;
    ResourceManager(ResourceManager&& obj) = delete;

    // Make post processing shaders
    static std::map<const std::string, std::shared_ptr<Shader>> makePostProcessingShaders(const std::string& path_to_folder);

    // Make shared ptrs
    static std::shared_ptr<Shader>               makeShaderProgram(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path);
    static std::shared_ptr<Texture>              makeTexture(std::string type, const std::string& texture_rel_path);
    static std::shared_ptr<Model>                makeModel(const std::string& model_rel_path);
    static std::shared_ptr<CylindricalBillboard> makeCylBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_path, std::vector<Vertex> verts);
    static std::shared_ptr<SphericalBillboard>   makeSphBillboard(glm::vec3 pos, glm::vec2 size, const std::string& texture_path, std::vector<Vertex> verts);

    static void displayLoadedObjects();
private:
    inline static const std::string relResPath = RES_PATH; // full path to res directory
    inline static const std::string VERTEX_FILE_EXTENSION = "vert";
    inline static const std::string FRAGMENT_FILE_EXTENSION = "frag";

    // Map of loaded resources (First - name of file, second - ptrs of objects)
    inline static std::map<std::string, std::shared_ptr<void>> m_LoadedObjects;

    static std::string getFileString(const std::string& file_rel_path); // gets file contents as a string
    static std::vector<std::shared_ptr<Texture>> pullTexturesFromMtl(const std::string& fullfilepath);
};