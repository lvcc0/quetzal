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

enum class ObjectType{SHADER, TEXTURE, MODEL, CYL_BILLBOARD, SPH_BILLBOARD};

class ResourceManager
{
private:
	//Main vars(constants or semiconstants)
	std::string relResPath; // full path to res directory
	const std::string POST_PROCESSING_VERTEX_FILE_NAME = "post_processing.vert";

public:
	// --- Maps of loaded objects --- //
	typedef std::map<const std::string, std::shared_ptr<Shader>> ShaderMap;
	ShaderMap shaderMap;

	typedef std::map<const std::string, std::shared_ptr<Model>> ModelMap;
	ModelMap modelMap;

	typedef std::map<const std::string, std::shared_ptr<Texture>> TextureMap;
	TextureMap textureMap;

	typedef std::map<const std::string, std::shared_ptr<CylindricalBillboard>> CylBillboardMap;
	CylBillboardMap cylBillboardMap;
	
	typedef std::map<const std::string, std::shared_ptr<SphericalBillboard>> SphBillboardMap;
	SphBillboardMap sphBillboardMap;
	// --- //

	// Constructor
	ResourceManager(const std::string& main_path);

	// No move or copy constructors allowed
	ResourceManager(const ResourceManager& obj) = delete;
	ResourceManager(const ResourceManager&& obj) = delete;

	// Print objects in maps
	void getObjectsInMaps(ObjectType objectType);

	// Make post processing shaders
	ShaderMap make_post_processing_shaders(std::string path_to_folder);

	// Save shared_ptrs into their maps specified above
	std::shared_ptr<Shader> make_shader_program(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path);
	std::shared_ptr<Texture> make_texture(std::string name, std::string type, const std::string& texture_rel_path);
	std::shared_ptr<Model> make_model(std::string name, const std::string& model_rel_path);
	std::shared_ptr<CylindricalBillboard> make_cyl_billboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path);
	std::shared_ptr<SphericalBillboard> make_sph_billboard(std::string name, glm::vec3 pos, glm::vec2 size, const std::string& texture_path);
private:
	std::string get_file_string(const std::string& file_rel_path); // gets file contents as a string
	std::vector<std::shared_ptr<Texture>> pull_textures_from_mtl(const std::string& fullfilepath);
};