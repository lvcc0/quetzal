#include "camera.h"

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_pos, m_pos + m_orientation, m_up);
}

void Camera::Inputs(GLFWwindow* window, float dt)
{
	// --- Keyboard --- //
	float velocity;
	(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? velocity = m_speed * dt * 3 : velocity = m_speed * dt;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_pos += velocity * m_orientation;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_pos -= velocity * glm::normalize(glm::cross(m_orientation, m_up));

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_pos -= velocity * m_orientation;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_pos += velocity * glm::normalize(glm::cross(m_orientation, m_up));

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		m_pos += velocity * m_up;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		m_pos -= velocity * m_up;
	// --- //
	
	// --- Mouse --- //
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (m_firstClick)
		{
			glfwSetCursorPos(window, (double)m_width / 2, (double)m_height / 2);
			m_firstClick = false;
		}

		double mouseX, mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = m_sens * (float)(mouseY - (m_height / 2)) / m_height;
		float rotY = m_sens * (float)(mouseX - (m_width / 2)) / m_width;

		glm::vec3 newOrientation = glm::rotate(m_orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_orientation, m_up)));

		if (abs(glm::angle(newOrientation, m_up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			m_orientation = newOrientation;

		m_orientation = glm::rotate(m_orientation, glm::radians(-rotY), m_up);

		glfwSetCursorPos(window, (double)m_width / 2, (double)m_height / 2);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_firstClick = true;
	}
	// --- //
}

void Camera::UpdateSize(int w, int h)
{
	m_width = w;
	m_height = h;
}