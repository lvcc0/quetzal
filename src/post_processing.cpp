#include "post_processing.h"

void FrameBufferMaker::make_vertexArray(GLuint& VAO, GLuint& VBO)
{
	// prepare VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), &quadVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glBindVertexArray(0);
	//-----------------------------//
}

void FrameBufferMaker::make_buffers(GLuint& frameBuffer, GLuint& textureFrameBuffer, GLuint& renderFrameBuffer, GLfloat width, GLfloat height)
{
	// making buffers
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &textureFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, textureFrameBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFrameBuffer, 0);

	glGenRenderbuffers(1, &renderFrameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderFrameBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderFrameBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR::FRAMEBUFFER WASN`T COMPLETED::ID " << frameBuffer << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


PostProcessing::PostProcessing(ShaderMap& shaderMap, GLfloat width, GLfloat height) {
	this->shaderMap = shaderMap;
	FrameBufferMaker::make_vertexArray(VAO, VBO);
	FrameBufferMaker::make_buffers(frameBuffer, textureFrameBuffer, renderFrameBuffer, width, height);
}

void PostProcessing::post_processing(std::string type_of_processing) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClear(GL_COLOR_BUFFER_BIT);

	std::shared_ptr<Shader> screen_shader;

	if (shaderMap.find(type_of_processing) != shaderMap.end()) {
		screen_shader = shaderMap.find(type_of_processing)->second;
		if (type_of_processing == "inversion_color") {
			screen_shader->Activate();
			screen_shader->setInt("screenTexture", 0);
		}
		else if (type_of_processing == "grayscale") {
			screen_shader->Activate();
			screen_shader->setInt("screenTexture", 0);
		}
	}
	else {
		std::cerr << "post processing shader::" << type_of_processing << " wasnt found" << std::endl;
	}

	screen_shader->Activate();
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, textureFrameBuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::deactivate() {
	// Bind the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	// Specify the color of the background
	glClearColor(0.77f, 0.73f, 0.77f, 1.0f);
	// Clean the back buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Enable depth testing since it's disabled when drawing the framebuffer rectangle
	glEnable(GL_DEPTH_TEST);
}

PostProcessing::~PostProcessing() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &frameBuffer);
	glDeleteBuffers(1, &textureFrameBuffer);
	glDeleteBuffers(1, &renderFrameBuffer);
	std::map<std::string, std::shared_ptr<Shader>>::iterator iter = shaderMap.begin();
	while (iter != shaderMap.end()) {
		iter->second->Delete();
		iter++;
	}
}