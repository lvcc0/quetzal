#include "shader.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary | std::ios::ate);
	if (in)
	{
		std::string contents;

		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);

		in.read(&contents[0], contents.size());
		in.close();
		
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vShaderFile, const char* fShaderFile)
{
	std::string vertexCode = get_file_contents(vShaderFile);
	std::string fragmentCode = get_file_contents(fShaderFile);

	const char* vShaderSrc = vertexCode.c_str();
	const char* fShaderSrc = fragmentCode.c_str();

	// SHADERS

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderSrc, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderSrc, NULL);
	glCompileShader(fragmentShader);

	// SHADER PROGRAM

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);

	compileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "shader compilation error: " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "shader linking error: " << type << "\n" << infoLog << std::endl;
		}
	}
}

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat2(const std::string& name, const glm::mat2& value)
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
