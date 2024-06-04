#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<fstream>
#include<sstream>
#include <map>
#include<memory>
#include<iostream>

#include "shader.h"
#include "model.h"

#include "C:/OpenGL/GameEngine/res/stb_image.h"

class ResourceManager {
	std::string relResPath;   /*in this variable full path to res folder*/

	std::string get_file_string(const std::string& file_rel_path); /*gets contents from file in string type*/ /*!!!method only for other methods!!!*/

	typedef std::map<std::string, std::shared_ptr<Shader>> ShaderMap;
	ShaderMap shaderMap;   

	typedef std::map<std::string, std::shared_ptr<Model>> ModelMap;
	ModelMap textureMap; 
public:
	ResourceManager(const std::string& main_path); /*init class*/
	/*these funcs return shared_ptrs and save them in their special maps */
	std::shared_ptr<Shader> make_shader_program(std::string name, const std::string& vertex_shader_rel_path, const std::string& fragment_shader_rel_path);
	std::shared_ptr<Model> make_model(std::string name, const std::string& model_rel_path);

	/*deleting move or copy constructors*/
	ResourceManager(const ResourceManager& obj) = delete;
	ResourceManager(const ResourceManager&& obj) = delete;
	/*---------------------------------------*/
};