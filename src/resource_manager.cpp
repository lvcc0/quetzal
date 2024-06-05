#include "resource_manager.h"

ResourceManager::ResourceManager(const std::string& main_path)
{
	unsigned end_of_string = main_path.find_last_of("/\\");
	relResPath = main_path.substr(0, end_of_string); // saving res path
};

std::string ResourceManager::get_file_string(const std::string& file_rel_path) {
	std::string file_path = relResPath + "/" + file_rel_path.c_str();  // full file path
	std::ifstream file(file_path, std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "File failed to open at: " << file_path << std::endl;
		return {};
	}
	
	// Returns file contents as a string
	std::stringstream readbuffer;
	readbuffer << file.rdbuf();
	return readbuffer.str();
};

std::shared_ptr<Shader> ResourceManager::make_shader_program(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path)
{
	// Getting shader files (.vert and .frag)
	std::string vertex_shader_src = get_file_string(vertex_shader_rel_path);
	std::string fragment_shader_src = get_file_string(fragment_shader_rel_path);

	std::shared_ptr<Shader>& shader_program = std::make_shared<Shader>(vertex_shader_src, fragment_shader_src);
	shaderMap.emplace(name, shader_program);

	return shader_program;
}

std::shared_ptr<Model> ResourceManager::make_model(std::string name, const std::string& model_rel_path)
{
	/*TODO*/
	/*Move file interactions from model to resource manager*/

	std::shared_ptr<Model>& model = std::make_shared<Model>(relResPath + "/" + model_rel_path);
	return model;
};
