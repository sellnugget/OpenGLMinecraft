#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Chunk.h"
#include <vector>
#include "Block.h"
class ChunkGenerator {
public:

	//this function takes in a chunk and makes a mesh out of quads
	static void CreateMesh(Chunk* chunk);

	static void GenerateStoneNoise(Chunk* chunk);

private:

};