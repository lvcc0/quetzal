#include <iostream>
#include <stb/stb_image.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl/camera.h"
#include "gl/texture.h"
#include "gl/shader.h"
#include "gl/VAO.h"
#include "gl/VBO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
void showGuiWindow();

float vertices[] = 
{    // positions         // texture    // normals
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f
};

glm::vec3 cubePositions[] =
{
	glm::vec3(4.0f,  -3.0f,  -2.0f),
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

const unsigned int WIN_WIDTH = 1280;
const unsigned int WIN_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool shouldDrawGui = false;

float FOV = 45.0f;

glm::vec3 objectScale(1.0f, 1.0f, 1.0f);

float materialShininess = 32.0f;

glm::vec3 lightPos(3.0f, 1.0f, 3.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightAmbient(1.0f, 1.0f, 1.0f);
glm::vec3 lightDiffuse(1.0f, 1.0f, 1.0f);
glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

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
		std::cout << "can't create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	gladLoadGL();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// SHADERS

	Shader shaderProgram("res/shaders/default.vert", "res/shaders/default.frag");
	Shader lightCubeShader("res/shaders/light.vert", "res/shaders/light.frag");

	VBO VBO1(vertices, sizeof(vertices));

	// Regular cubes

	VAO cubeVAO;
	cubeVAO.Bind();

	// Position attribute
	cubeVAO.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	// Texture coord attribute
	cubeVAO.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// Normal attribute
	cubeVAO.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	// Light sources

	VAO lightVAO;
	lightVAO.Bind();

	// Position attribute
	lightVAO.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);

	cubeVAO.Unbind();
	lightVAO.Unbind();
	VBO1.Unbind();
	
	// TEXTURES

	Texture diffuseMap("res/textures/container.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture specularMap("res/textures/container_spec.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);

	// SHADER CONFIG
	shaderProgram.Activate();
	shaderProgram.setInt("material.diffuse", 0);
	shaderProgram.setInt("material.specular", 1);

	// MAIN LOOP

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (shouldDrawGui)
		{
			showGuiWindow();
		}

		float curFrame = (float)glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);
		
		// lightPos.x = sin(curFrame) * 3.0f;
		// lightPos.y = sin(curFrame) * 1.0f;
		// lightPos.z = cos(curFrame) * 3.0f;

		diffuseMap.Bind();
		specularMap.Bind();

		cubeVAO.Bind();
		shaderProgram.Activate();

		glm::mat4 proj = glm::perspective(glm::radians(FOV), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = cam.getViewMatrix();

		shaderProgram.setVec3("viewPos", cam.Position);

		shaderProgram.setVec3("light.position", cam.Position);
		shaderProgram.setVec3("light.direction", cam.Orientation);

		shaderProgram.setVec3("light.ambient", lightAmbient);
		shaderProgram.setVec3("light.diffuse", lightDiffuse);
		shaderProgram.setVec3("light.specular", lightSpecular);

		shaderProgram.setFloat("light.constant", 1.0f);
		shaderProgram.setFloat("light.linear", 0.14f);
		shaderProgram.setFloat("light.quad", 0.07f);

		shaderProgram.setFloat("light.innerCutoff", glm::cos(glm::radians(12.5f)));
		shaderProgram.setFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));

		shaderProgram.setFloat("material.shininess", materialShininess);

		shaderProgram.setMat4("projection", proj);
		shaderProgram.setMat4("view", view);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(curFrame * i), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, objectScale);

			shaderProgram.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightVAO.Bind();
		lightCubeShader.Activate();

		lightCubeShader.setVec3("objectColor", lightColor);

		lightCubeShader.setMat4("projection", proj);
		lightCubeShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightCubeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// CLEAN UP

	//VBOs
	VBO1.Delete();
	
	// VAOs
	cubeVAO.Delete();
	lightVAO.Delete();
	
	// Shaders
	shaderProgram.Delete();
	lightCubeShader.Delete();
	
	// Textures
	diffuseMap.Delete();
	specularMap.Delete();

	// ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	cam.UpdateSize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		shouldDrawGui = !shouldDrawGui;
}

void processInput(GLFWwindow* window)
{
	cam.Inputs(window, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	glPolygonMode(GL_FRONT_AND_BACK, (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) ? GL_LINE : GL_FILL);
}

void showGuiWindow()
{
	ImGui::Begin("Stuff");

	ImGui::SliderFloat3("Object Scale", (float*)&objectScale, 0.0f, 3.0f);

	ImGui::SeparatorText("Light");
	ImGui::ColorEdit3("Light Color", (float*)&lightColor);
	ImGui::ColorEdit3("Light Ambient", (float*)&lightAmbient);
	ImGui::ColorEdit3("Light Diffuse", (float*)&lightDiffuse);
	ImGui::ColorEdit3("Light Specular", (float*)&lightSpecular);

	ImGui::SeparatorText("Material");
	ImGui::SliderFloat("Material Shininess", (float*)&materialShininess, 1.0f, 256.0f);

	ImGui::SeparatorText("Screen");
	ImGui::SliderFloat("FOV", &FOV, 0.0f, 120.0f);

	ImGui::Text("%.3f ms (%.1f FPS)", deltaTime * 1000.0f, 1.0f / deltaTime);

	ImGui::End();
}