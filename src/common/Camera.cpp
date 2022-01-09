#include "Camera.h"
using namespace glm;

namespace 
{
	glm::vec3 camera_position = vec3(0, 1, -6);
	glm::vec3 camera_direction;
	float yaw_angle = 180;
	GLFWwindow *window;
	float pitch_angle = 0;

	bool moving_forward = false;
	bool moving_backward = false;
	bool moving_left = false;
	bool moving_right = false;

	void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
	{
		static bool first_mouse = true;
		static float last_x = 0;
		static float last_y = 0;
		if (first_mouse) // эта переменная была проинициализирована значением true
		{
			last_x = x_pos;
			last_y = y_pos;
			first_mouse = false;
		}
		GLfloat xoffset = x_pos - last_x;
		GLfloat yoffset = last_y - y_pos;
		last_x = x_pos;
		last_y = y_pos;

		yaw_angle += xoffset * 0.1f;
		pitch_angle += yoffset * 0.1f;

		camera_direction = normalize(vec3(
			cos(radians(-pitch_angle / 2)) * cos(radians(-yaw_angle / 2)),
			sin(radians(-pitch_angle / 2)),
			cos(radians(-pitch_angle / 2)) * sin(radians(-yaw_angle / 2))
		));
	}
	void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_W)
		{
			if (action == GLFW_PRESS)
				moving_forward = true;
			else if (action == GLFW_RELEASE)
				moving_forward = false;
		}
		if (key == GLFW_KEY_S)
		{
			if (action == GLFW_PRESS)
				moving_backward = true;
			else if (action == GLFW_RELEASE)
				moving_backward = false;
		}
		if (key == GLFW_KEY_A)
		{
			if (action == GLFW_PRESS)
				moving_left = true;
			else if (action == GLFW_RELEASE)
				moving_left = false;
		}
		if (key == GLFW_KEY_D)
		{
			if (action == GLFW_PRESS)
				moving_right = true;
			else if (action == GLFW_RELEASE)
				moving_right = false;
		}
	}
}

glm::mat4 Camera::getView()
{

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));
	glm::vec3 camera_up = glm::cross(camera_direction, camera_right);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return mylookAt(camera_position, camera_position + camera_direction, vec3(0, 1, 0));
}

glm::vec3 Camera::getPosition()
{
	return camera_position;
}

void Camera::update(float dt)
{
	bool moved = false;
	vec3 moving_direction = vec3(0);

	if (moving_forward)
	{
		moving_direction -= camera_direction;
		moved = true;
	}
	if (moving_backward)
	{
		moving_direction += camera_direction;
		moved = true;

	}
	if (moving_left)
	{
		moving_direction -= cross(camera_direction, vec3(0, 1, 0));
		moved = true;
	}

	if (moving_right)
	{
		moving_direction += cross(camera_direction, vec3(0, 1, 0));
		moved = true;
	}
	if (moved)
		camera_position += normalize(moving_direction) * 0.01f;

}

Camera::Camera(GLFWwindow *glfw_window)
{
	glfwSetCursorPosCallback(glfw_window, mouse_callback);
	glfwSetKeyCallback(glfw_window, key_callback);
	window = glfw_window;

}


glm::mat4 Camera::mylookAt(glm::vec3 eye, glm::vec3 at, glm::vec3 up)
{
	vec3 v = normalize(at - eye);  // view direction vector
	vec3 n = normalize(cross(v, up)); // perpendicular vector
	vec3 u = normalize(cross(n, v)); // "new" up vector
	v = -v;

	mat4 result = mat4(
		n[0], n[1], n[2], -dot(n, eye),
		u[0], u[1], u[2], -dot(u, eye),
		v[0], v[1], v[2], -dot(v, eye),
		0.0, 0.0, 0.0, 1.0
	);

	return result;
}
