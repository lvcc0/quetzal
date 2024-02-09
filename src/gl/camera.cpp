#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(Position, Position + Orientation, Up);
}

void Camera::Inputs(GLFWwindow* window, float dt)
{
	// KEYBOARD INPUTS

	float velocity;
	(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? velocity = speed * dt * 2 : velocity = speed * dt;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += velocity * Orientation;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position -= velocity * glm::normalize(glm::cross(Orientation, Up));

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= velocity * Orientation;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += velocity * glm::normalize(glm::cross(Orientation, Up));

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		Position += velocity * Up;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		Position -= velocity * Up;

	// MOUSE INPUTS

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, width / 2, height / 2);
			firstClick = false;
		}

		double mouseX, mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sens * (float)(mouseY - (height / 2)) / height;
		float rotY = sens * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			Orientation = newOrientation;

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		glfwSetCursorPos(window, width / 2, height / 2);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}

void Camera::UpdateSize(int w, int h)
{
	width = w;
	height = h;
}