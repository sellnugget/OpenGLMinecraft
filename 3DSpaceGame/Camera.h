#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <vector>
class Camera
{
public:
	float Fov = 100;
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Rotation = glm::vec3(-90.0f, 0.0f, 90.0f);



	//the camera will go through all of these shaders and update the matrix info every frame
	GL::Shader* shader;
	Camera();
	~Camera();
private:
	int Instance;

	void Update();
};

