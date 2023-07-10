#pragma once
#include "Shader.h"
class Mesh
{
public:
	unsigned int Reference = -1000;
	glm::vec3 Position;
	glm::vec3 Origin;
	glm::vec3 Rotation;
	glm::vec3 Scale = glm::vec3(1,1,1);
	GL::Shader* shader;
};

