#pragma once

#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include "shader.h"
#include "model.h"


class ResourceManager
{
public:
	typedef std::map<std::string, std::shared_ptr<Shader>> ShaderMap;
	ShaderMap shaderMap;

	typedef std::map<std::string, std::shared_ptr<Model>> ModelMap;
	ModelMap textureMap;

	// Constructor
	ResourceManager(const std::string& main_path);

	// No move or copy constructors allowed
	ResourceManager(const ResourceManager& obj) = delete;
	ResourceManager(const ResourceManager&& obj) = delete;

	// Save shared_ptrs into their maps specified above
	std::shared_ptr<Shader> make_shader_program(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path);
	std::shared_ptr<Model> make_model(std::string name, const std::string& model_rel_path);

private:
	std::string relResPath; // full path to res directory
	std::string get_file_string(const std::string& file_rel_path); // gets file contents as a string
};