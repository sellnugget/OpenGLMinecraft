#pragma once
#include <glm/glm.hpp>
#include "Chunk.h"
#define RAY_MISSED -1.0f


struct RayHit {
	float Distance;
	glm::vec3 Normal;
	glm::vec3 HitPoint;
	glm::ivec3 BlockHit;
};

RayHit RayChunkCast(Chunk* chunk, glm::vec3 RayDir, glm::vec3 RayOrigin, float distance);