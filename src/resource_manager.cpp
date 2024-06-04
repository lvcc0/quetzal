#include "resource_manager.h"
#include "resource_manager.h"


/*init res object, put full path to res folder into variable*/
ResourceManager::ResourceManager(const std::string& main_path) {
	unsigned end_of_string = main_path.find_last_of("/\\");
	relResPath = main_path.substr(0, end_of_string);
};
/*----------------*/

std::string ResourceManager::get_file_string(const std::string& file_rel_path) {
	std::string file_path = relResPath + "/" + file_rel_path.c_str();  /*full path of file*/
	std::ifstream File;
	File.open(file_path, std::ios::in | std::ios::binary);
	/*checking if file opened(if not, return nothing)*/
	if (!File.is_open()) {
		std::cerr << "File in path " + file_path + " isn`t open" << std::endl;
		return{};
	}
	/*-------------------*/
	/*returns contents of file in string type*/
	std::stringstream readbuffer;
	readbuffer << File.rdbuf();
	return readbuffer.str();
	/*-----------------------*/
};

std::shared_ptr<Shader> ResourceManager::make_shader_program(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path)
{
	/*Getting shaders*/
	std::string vertex_shader_src = get_file_string(vertex_shader_rel_path);
	std::string fragment_shader_src = get_file_string(fragment_shader_rel_path);
	/*---------------*/

	std::shared_ptr<Shader>& shader_program = std::make_shared<Shader>(vertex_shader_src, fragment_shader_src);
	shaderMap.emplace(name, shader_program);   /*inserts shader_program into shaderMap*/
	return shader_program;
}
std::shared_ptr<Model> ResourceManager::make_model(std::string name, const std::string& model_rel_path)
{
	/*TODO*/
	/*Move file interactions from model to resource manager*/
	std::shared_ptr<Model>& model = std::make_shared<Model>(relResPath + "/" + model_rel_path);
	return model;
};
