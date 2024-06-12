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

class ResourceManager
{
public:

	// Delete all constructors
	ResourceManager() = delete;
	ResourceManager(const ResourceManager& obj) = delete;
	ResourceManager(const ResourceManager&& obj) = delete;

	// Make post processing shaders
	static std::map<const std::string, std::shared_ptr<Shader>> make_post_processing_shaders(const std::string& path_to_folder);

	// Save shared_ptrs into their maps specified above
	static std::shared_ptr<Shader> make_shader_program(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path);
	static std::shared_ptr<Texture> make_texture(std::string name, std::string type, const std::string& texture_rel_path);
	static std::shared_ptr<Model> make_model(std::string name, const std::string& model_rel_path);
	static std::shared_ptr<CylindricalBillboard> make_cyl_billboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path);
	static std::shared_ptr<SphericalBillboard> make_sph_billboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path);

private:
	inline static const std::string relResPath = RES_PATH; // full path to res directory
	inline static const std::string VERTEX_FILE_EXTENSION = "vert";
	inline static const std::string FRAGMENT_FILE_EXTENSION = "frag";

	static std::string get_file_string(const std::string& file_rel_path); // gets file contents as a string
	static std::vector<std::shared_ptr<Texture>> pull_textures_from_mtl(const std::string& fullfilepath);
};