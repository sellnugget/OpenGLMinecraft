#include "Camera.h"
#include "Time.h"
#include "Input.h"
#include "Window.h"
#include "GameEvent.h"
Camera::Camera()
{
	GameEvents::Update.Sub([this]() {Update(); }, &Instance);
}

Camera::~Camera()
{
	GameEvents::Update.Unsub(Instance);
}

void Camera::Update()
{
	float cameraSpeed = 4.317 * Time::GetDeltaTime();

	if (Input::Pressed(GLFW_KEY_LEFT_SHIFT)) {
		cameraSpeed *= 20;
	}
	glm::vec3 Increase = glm::vec3(0, 0, 0);

	if (Input::Pressed('W')) {
		Increase += cameraSpeed * glm::normalize(cameraFront * glm::vec3(1,0,1));
	}
	if (Input::Pressed('S')) {
		Increase -= cameraSpeed * glm::normalize(cameraFront * glm::vec3(1, 0, 1));
	}
	if (Input::Pressed('A')) {
		Increase -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (Input::Pressed('D')) {
		Increase += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	Increase.y = 0;

	Position += Increase;
	float LookSpeed = Time::GetDeltaTime() * 60;
	Rotation.x += Input::GetMouseXAxis() * LookSpeed;
	Rotation.y -= Input::GetMouseYAxis() * LookSpeed;

	if (Rotation.y > 89.0f)
		Rotation.y = 89.0f;
	if (Rotation.y < -89.0f)
		Rotation.y = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(Rotation.x)) * cos(glm::radians(Rotation.y));
	front.y = sin(glm::radians(Rotation.y));
	front.z = sin(glm::radians(Rotation.x)) * cos(glm::radians(Rotation.y));
	cameraFront = glm::normalize(front);
	

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(Fov), ((float)Window::Width) / Window::Height, 0.1f, 10000.0f);
	shader->setUniform("projection", projection);


	glm::mat4 View = glm::lookAt(Position, Position + cameraFront, cameraUp);
	shader->setUniform("View", View);
}
