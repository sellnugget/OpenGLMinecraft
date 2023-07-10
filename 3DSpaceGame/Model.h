#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcord;
};

class Model
{
public:
	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCords;
	//first array contains each face which matches the normals
	std::vector<int> VertIndices;
	std::vector<int> NormIndices;
	std::vector<int> TexIndices;

	//turns the indices to vertices and there positions to normals
	Vertex* GetMeshData();
	int TotalIndices = 0;


	void LoadObjModel(std::string FileName);

};

