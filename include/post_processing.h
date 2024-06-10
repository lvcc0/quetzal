#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include <shader.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// main idea: post processing uses custom frame buffers and using them for post processing shaders
// MAIN RULES FOR EXISTING FRAME BUFFERS::
// 1) At least one buffer (color, depth or stencil) must be connected.
// 2) At least one color attachment must be present.
// 3) All connections must also be completed(provided with allocated memory).
// 4) Each buffer must have the same number of samples(now idk what`s this).
class FrameBufferMaker {
	// Quad verts
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
	inline static std::vector<float> quadVertices = { 
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	
public:
	//Make VAO
	static void make_vertexArray(GLuint& VAO, GLuint& VBO);

	// Make buffers
	static void make_buffers(GLuint &frameBuffer, GLuint &textureFrameBuffer, GLuint &renderFrameBuffer, GLfloat width, GLfloat height);

	//Deleting all constructors
	FrameBufferMaker() = delete;
	FrameBufferMaker(const FrameBufferMaker& obj) = delete;
	FrameBufferMaker(const FrameBufferMaker&& obj) = delete;
};

class PostProcessing {
	// buffers
	GLuint VAO, VBO;
	GLuint frameBuffer, textureFrameBuffer, renderFrameBuffer;
	// map of shaders
	typedef std::map<const std::string, std::shared_ptr<Shader>> ShaderMap;
	ShaderMap shaderMap;

public:
	// constructor
	PostProcessing(ShaderMap& shaderMap, GLfloat width, GLfloat height);
	//destructor
	~PostProcessing();
	// post processing funcs
	void post_processing(std::string type_of_processing);
	// deactivating post processing(must be used BEFORE any post processing funcs and objects drawing)
	void deactivate();
};