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
#include "gl/lights.h"
#include "gl/shader.h"
#include "gl/VAO.h"
#include "gl/VBO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
void showGuiWindow();

const unsigned int WIN_WIDTH = 1280;
const unsigned int WIN_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool shouldDrawGui = false;

float FOV = 45.0f;

glm::vec3 objectScale(1.0f, 1.0f, 1.0f);
float materialShininess = 32.0f;

Camera cam(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 5.0f));

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

DirLight dirLights[] =
{
	DirLight(
		true,
		"dirLight0",
		glm::vec3(-0.2f, -1.0f, -0.3f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	),
	DirLight(
		true,
		"dirLight1",
		glm::vec3(0.2f, -1.0f, 0.3f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	)
};

PointLight pointLights[] =
{
	PointLight(
		true,
		"pointLight0",
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		0.09f,
		0.032f,
		glm::vec3(1.0f, 0.0f, 0.0f)
	),
	PointLight(
		true,
		"pointLight1",
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		0.09f,
		0.032f,
		glm::vec3(0.0f, 1.0f, 0.0f)
	)
};

SpotLight spotLights[] =
{
	SpotLight(
		true,
		"flashlight",
		cam.Position,
		cam.Orientation,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		0.09f,
		0.032f,
		12.5f,
		15.0f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	),
	SpotLight(
		true,
		"spotLight0",
		glm::vec3(5.0f, 0.0f, -3.5f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		0.09f,
		0.032f,
		7.5f,
		10.0f,
		glm::vec3(0.0f, 0.0f, 1.0f)
	)
};

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
			showGuiWindow();

		float curFrame = (float)glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);
		
		diffuseMap.Bind();
		specularMap.Bind();

		cubeVAO.Bind();
		shaderProgram.Activate();

		glm::mat4 proj = glm::perspective(glm::radians(FOV), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = cam.getViewMatrix();

		shaderProgram.setVec3("viewPos", cam.Position);
		shaderProgram.setFloat("material.shininess", materialShininess);

		for (unsigned int i = 0; i < IM_ARRAYSIZE(dirLights); i++)
			dirLights[i].UpdateUni(shaderProgram, i);

		for (unsigned int i = 0; i < IM_ARRAYSIZE(pointLights); i++)
			pointLights[i].UpdateUni(shaderProgram, i);

		for (unsigned int i = 0; i < IM_ARRAYSIZE(spotLights); i++)
		{
			if (spotLights[i].Name == "flashlight")
			{
				spotLights[i].Position = cam.Position;
				spotLights[i].Direction = cam.Orientation;
			}
			spotLights[i].UpdateUni(shaderProgram, i);
		}

		shaderProgram.setMat4("projection", proj);
		shaderProgram.setMat4("view", view);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(curFrame * i), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, objectScale);

			shaderProgram.setMat4("inversed", glm::inverse(model));
			shaderProgram.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightVAO.Bind();
		lightCubeShader.Activate();

		lightCubeShader.setMat4("projection", proj);
		lightCubeShader.setMat4("view", view);

		// Point Lights
		for (unsigned int i = 0; i < IM_ARRAYSIZE(pointLights); i++)
		{
			if (pointLights[i].Enabled)
			{
				glm::mat4 model = glm::mat4(1.0f);

				model = glm::translate(model, pointLights[i].Position);
				model = glm::scale(model, glm::vec3(0.3f));

				lightCubeShader.setVec3("objectColor", pointLights[i].Color);
				lightCubeShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// Spot Lights
		for (unsigned int i = 0; i < IM_ARRAYSIZE(spotLights); i++)
		{
			if (spotLights[i].Enabled)
			{
				glm::mat4 model = glm::mat4(1.0f);

				model = glm::translate(model, spotLights[i].Position);
				model = glm::scale(model, glm::vec3(0.1f));

				lightCubeShader.setVec3("objectColor", spotLights[i].Color);
				lightCubeShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		
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

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
		spotLights[0].Enabled = !spotLights[0].Enabled;
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
	
	if (ImGui::CollapsingHeader("Directional Lights"))
	{
		static int dirComboItem = 0;

		if (ImGui::BeginCombo("Source", dirLights[dirComboItem]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(dirLights); i++)
			{
				const bool is_selected = (dirComboItem == i);
				if (ImGui::Selectable(dirLights[i], is_selected))
					dirComboItem = i;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		
		ImGui::Checkbox("Enabled", &dirLights[dirComboItem].Enabled);
		ImGui::SliderFloat3("Direction", (float*)&dirLights[dirComboItem].Direction, -1.0f, 1.0f);

		ImGui::Separator();
		ImGui::ColorEdit3("Ambient", (float*)&dirLights[dirComboItem].Ambient);
		ImGui::ColorEdit3("Diffuse", (float*)&dirLights[dirComboItem].Diffuse);
		ImGui::ColorEdit3("Specular", (float*)&dirLights[dirComboItem].Specular);

		ImGui::Separator();
		ImGui::ColorEdit3("Color", (float*)&dirLights[dirComboItem].Color);
		
		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Point Lights"))
	{
		static int pointComboItem = 0;

		if (ImGui::BeginCombo("Source", pointLights[pointComboItem]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(pointLights); i++)
			{
				const bool is_selected = (pointComboItem == i);
				if (ImGui::Selectable(pointLights[i], is_selected))
					pointComboItem = i;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Checkbox("Enabled", &pointLights[pointComboItem].Enabled);
		ImGui::DragFloat3("Position", (float*)&pointLights[pointComboItem].Position, 0.1f);

		ImGui::Separator();
		ImGui::ColorEdit3("Ambient", (float*)&pointLights[pointComboItem].Ambient);
		ImGui::ColorEdit3("Diffuse", (float*)&pointLights[pointComboItem].Diffuse);
		ImGui::ColorEdit3("Specular", (float*)&pointLights[pointComboItem].Specular);

		ImGui::Separator();
		ImGui::InputFloat("Constant", (float*)&pointLights[pointComboItem].Constant, 0.1f, 1.0f);
		ImGui::InputFloat("Linear", (float*)&pointLights[pointComboItem].Linear, 0.01f, 0.1f);
		ImGui::InputFloat("Quadratic", (float*)&pointLights[pointComboItem].Quad, 0.001f, 0.01f);

		ImGui::Separator();
		ImGui::ColorEdit3("Color", (float*)&pointLights[pointComboItem].Color);

		ImGui::Separator();
	}

	if (ImGui::CollapsingHeader("Spot Lights"))
	{
		static int spotComboItem = 0;

		if (ImGui::BeginCombo("Source", spotLights[spotComboItem]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(spotLights); i++)
			{
				const bool is_selected = (spotComboItem == i);
				if (ImGui::Selectable(spotLights[i], is_selected))
					spotComboItem = i;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Checkbox("Enabled", &spotLights[spotComboItem].Enabled);
		ImGui::DragFloat3("Position", (float*)&spotLights[spotComboItem].Position, 0.1f);
		ImGui::SliderFloat3("Direction", (float*)&spotLights[spotComboItem].Direction, -1.0f, 1.0f);

		ImGui::Separator();
		ImGui::ColorEdit3("Ambient", (float*)&spotLights[spotComboItem].Ambient);
		ImGui::ColorEdit3("Diffuse", (float*)&spotLights[spotComboItem].Diffuse);
		ImGui::ColorEdit3("Specular", (float*)&spotLights[spotComboItem].Specular);

		ImGui::Separator();
		ImGui::InputFloat("Constant", (float*)&spotLights[spotComboItem].Constant, 0.1f, 1.0f);
		ImGui::InputFloat("Linear", (float*)&spotLights[spotComboItem].Linear, 0.01f, 0.1f);
		ImGui::InputFloat("Quadratic", (float*)&spotLights[spotComboItem].Quad, 0.001f, 0.01f);

		ImGui::Separator();
		ImGui::DragFloat("Inner Angle", (float*)&spotLights[spotComboItem].InnerCutoff, 0.5f, 0.0f, 90.0f);
		ImGui::DragFloat("Outer Angle", (float*)&spotLights[spotComboItem].OuterCutoff, 0.5f, 0.0f, 90.0f);

		ImGui::Separator();
		ImGui::ColorEdit3("Color", (float*)&spotLights[spotComboItem].Color);

		ImGui::Separator();
	}

	ImGui::SeparatorText("Material");
	ImGui::SliderFloat("Material Shininess", (float*)&materialShininess, 1.0f, 256.0f);

	ImGui::SeparatorText("Screen");
	ImGui::SliderFloat("FOV", &FOV, 0.0f, 120.0f);

	ImGui::Text("%.3f ms (%.1f FPS)", deltaTime * 1000.0f, 1.0f / deltaTime);

	ImGui::End();
}