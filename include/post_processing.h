#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include <shader.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class FrameBufferMaker
{	
public:
	// Make VAO
	static void makeVertexArray(GLuint& VAO, GLuint& VBO);

	// Make buffers
	static void makeBuffers(GLuint& frameBuffer, GLuint& textureFrameBuffer, GLuint& renderFrameBuffer, GLfloat width, GLfloat height);

	// Deleting all constructors
	FrameBufferMaker() = delete;
	FrameBufferMaker(const FrameBufferMaker& obj) = delete;
	FrameBufferMaker(const FrameBufferMaker&& obj) = delete;

private:
	inline static std::vector<float> quadVertices = { // a quad that fills the entire screen
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,

		-1.0f,  1.0f,  0.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f
	};
};

class PostProcessing
{
public:
	// Map of shaders
	std::map<const std::string, std::shared_ptr<Shader>> shaderMap;

	// Constructor
	PostProcessing(std::map<const std::string, std::shared_ptr<Shader>>& shaderMap, GLfloat width, GLfloat height);

	// Destructor
	~PostProcessing();

	// Postprocessing funcs
	void activate(const std::shared_ptr<Shader>& screen_shader);

	// Deactivating postprocessing (must be used BEFORE any postprocessing funcs and objects drawing)
	void deactivate();

private:
	// Buffers
	GLuint VAO, VBO;
	GLuint m_Framebuffer, m_TextureFramebuffer, m_RenderFramebuffer;
};