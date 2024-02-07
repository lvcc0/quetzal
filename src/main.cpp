#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl/camera.h"
#include "gl/texture.h"
#include "gl/shader.h"
#include "gl/VAO.h"
#include "gl/VBO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float vertices[] = 
{
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] =
{
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

const unsigned int WIN_WIDTH = 640;
const unsigned int WIN_HEIGHT = 480;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float FOV = 45.0f;

Camera cam(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 5.0f));

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "window", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "No window :(" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gladLoadGL();

	// SHADERS

	Shader shaderProgram("res/shaders/default.vs", "res/shaders/default.fs");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));

	// Position attribute
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	// Texture coord attribute
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	
	// TEXTURES

	Texture pepe("res/textures/pepe.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	pepe.texUnit(shaderProgram, "tex0", 0);

	// MAIN LOOP

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float curFrame = (float)glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);
		
		pepe.Bind();
		VAO1.Bind();
		
		shaderProgram.Activate();

		glm::mat4 proj = glm::perspective(glm::radians(FOV), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = cam.getViewMatrix();

		shaderProgram.setMat4("projection", proj);
		shaderProgram.setMat4("view", view);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(curFrame * i), glm::vec3(1.0f, 0.3f, 0.5f));
			if (i % 2 == 0)
				model = glm::scale(model, glm::vec3(sin(curFrame), sin(curFrame), sin(curFrame)));

			shaderProgram.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// CLEAN UP

	VAO1.Delete();
	VBO1.Delete();
	shaderProgram.Delete();
	pepe.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	cam.Inputs(window, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		FOV += 5.0f;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		FOV -= 5.0f;

	glPolygonMode(GL_FRONT_AND_BACK, (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) ? GL_LINE : GL_FILL);
}