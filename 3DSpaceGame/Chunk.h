#pragma once
#include <glm/glm.hpp>
#include "Mesh.h"
#include <vector>
#include "Texture2D.h"
#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 256
#define CHUNK_DEPTH 16
#define CHUNK_SIZE CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH
class Chunk
{
	struct FastQuad {
	//0 is up
	//1 is down
	//2 is right
	//3 is left
	//4 is backward
	//5 is forward
		uint8_t Direction;

		int BlockPosition;
		glm::vec3 FastNormal() {
			switch (Direction) {
			case 0:
				return glm::vec3(0,1,0);
			case 1:
				return glm::vec3(0, -1, 0);
			case 2:
				return glm::vec3(1, 0, 0);
			case 3:
				return glm::vec3(-1, 0, 0);
			case 4:
				return glm::vec3(0, 0, 1);
			case 5:
				return glm::vec3(0, 0, -1);
			}
		}
		glm::vec3 Positon() {
			glm::vec3 rtnVector;
			rtnVector.z = BlockPosition % CHUNK_DEPTH;
			rtnVector.y = (BlockPosition / CHUNK_DEPTH) % CHUNK_HEIGHT;
			rtnVector.x = BlockPosition / (CHUNK_HEIGHT * CHUNK_DEPTH);
			return rtnVector;
		}
	};


public:

	Mesh mesh;

	//3d array that stores id of all blocks in the chunk
	int ChunkData[CHUNK_SIZE];
	std::vector<FastQuad> Quads;

	Texture2D ChunkTexture;

	Chunk() {
		//sets all values to zero
		for (int i = 0; i < CHUNK_SIZE; i++) {
			ChunkData[i] = 0;
		}
	}

	int GetBlock(glm::ivec3 Position) {

		if (Position.x >= CHUNK_WIDTH || Position.x < 0) {
			return 0;
		}
		if (Position.y >= CHUNK_HEIGHT || Position.y < 0) {
			return 0;
		}
		if (Position.z >= CHUNK_DEPTH || Position.z < 0) {
			return 0;
		}
		return ChunkData[(CHUNK_WIDTH * CHUNK_HEIGHT * Position.z) + (CHUNK_WIDTH * Position.y) + Position.x];
	}
	void SetBlock(int ID, glm::ivec3 Position) {
		if (Position.x >= CHUNK_WIDTH || Position.x < 0) {
			throw std::out_of_range::exception();
		}
		if (Position.y >= CHUNK_HEIGHT || Position.y < 0) {
			throw std::out_of_range::exception();
		}
		if (Position.z >= CHUNK_DEPTH || Position.z < 0) {
			throw std::out_of_range::exception();
		}
		ChunkData[(CHUNK_WIDTH * CHUNK_HEIGHT * Position.z) + (CHUNK_WIDTH * Position.y) + Position.x] = ID;
	}

};

