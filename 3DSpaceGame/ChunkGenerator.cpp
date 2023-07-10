#include "ChunkGenerator.h"
#include "glm/glm.hpp"
#include "Chunk.h"
#include "Texture2D.h"

#include <GL/glew.h>
void ChunkGenerator::CreateMesh(Chunk* chunk)
{

	
	std::vector<float> VertexData;

	if (chunk->mesh.Reference > -1) {
		Renderer::DeleteMesh(chunk->mesh.Reference);
		chunk->ChunkTexture.Delete();
	}

	
	Texture2D texture;
	texture.LoadData(chunk->ChunkData, CHUNK_WIDTH, CHUNK_HEIGHT * CHUNK_DEPTH, GL_RGBA, GL_UNSIGNED_BYTE, 1);
	chunk->mesh.shader->setUniform("Chunk", 1);
	chunk->mesh.shader->setUniform("Chunkwidth", CHUNK_WIDTH);
	chunk->mesh.shader->setUniform("Chunkdepth", CHUNK_DEPTH);
	chunk->mesh.shader->setUniform("Chunkheight", CHUNK_HEIGHT);
	chunk->ChunkTexture = texture;


	glm::vec3 positions[] = {
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 0),
		glm::vec3(1, 1, 1),
		glm::vec3(0, 1, 1),
		glm::vec3(0, 1, 0),
	};
	int DownQuad[4] = { 4, 3, 2, 1 };
	int UpQuad[4] = { 6, 7, 8, 5 };
	int RightQuad[4] = {2, 6, 5, 1};
	int BackQuad[4] = { 3, 7, 6, 2 };
	int LeftQuad[4] = {4, 8, 7, 3};
	int FrontQuad[4] = { 1, 5,8, 4 };


	float TexCords[] = {
		
		0.000000, 1.000000,
		0.000000, 0.000000,
		1.000000, 0.000000,
		1.000000, 1.000000,
	};
	chunk->Quads.clear();

	for (int y = 0; y < CHUNK_HEIGHT; y++) {
		for (int x = 0; x < CHUNK_WIDTH; x++) {
			for (int z = 0; z < CHUNK_DEPTH; z++) {


				if (BlockTypes::blocks[chunk->GetBlock(glm::ivec3(x, y, z))].Transparent) {
					//check around 

					std::vector<int*> QuadDirections;
					std::vector<int> TextureIDs;
					std::vector<glm::vec3> normal;
					std::vector<glm::ivec3> ipos;

					if (chunk->GetBlock(glm::ivec3(x+1,y,z)) != 0) {
						QuadDirections.push_back((int*)&LeftQuad);
						ipos.push_back(glm::ivec3(x + 1, y, z));
						normal.push_back(glm::vec3(-1, 0, 0));
						int blockID = chunk->GetBlock(glm::ivec3(x + 1, y, z));
						TextureIDs.push_back(BlockTypes::blocks[blockID].Left);
						chunk->Quads.push_back({2, (CHUNK_WIDTH * CHUNK_HEIGHT * z) + (CHUNK_WIDTH * y) + (x + 1)});
					}
					if (chunk->GetBlock(glm::ivec3(x - 1, y, z)) != 0) {
						QuadDirections.push_back((int*)&RightQuad);
						ipos.push_back(glm::ivec3(x - 1, y, z));
						normal.push_back(glm::vec3(1, 0, 0));
						int blockID = chunk->GetBlock(glm::ivec3(x - 1, y, z));

						TextureIDs.push_back(BlockTypes::blocks[blockID].Right);
						chunk->Quads.push_back({ 3, (CHUNK_WIDTH * CHUNK_HEIGHT * z) + (CHUNK_WIDTH * y) + (x - 1) });
					}
					if (chunk->GetBlock(glm::ivec3(x, y + 1, z)) != 0) {
						QuadDirections.push_back((int*)&DownQuad);
						ipos.push_back(glm::ivec3(x, y + 1, z));
						normal.push_back(glm::vec3(0, -1, 0));
						int blockID = chunk->GetBlock(glm::ivec3(x, y + 1, z));
	
						TextureIDs.push_back(BlockTypes::blocks[blockID].Bottom);
						chunk->Quads.push_back({ 1, (CHUNK_WIDTH * CHUNK_HEIGHT * z) + (CHUNK_WIDTH * (y + 1)) + (x) });
					}
					if (chunk->GetBlock(glm::ivec3(x, y - 1, z)) != 0) {
						QuadDirections.push_back((int*)&UpQuad);
						ipos.push_back(glm::ivec3(x, y - 1, z));
						normal.push_back(glm::vec3(0, 1, 0));
						int blockID = chunk->GetBlock(glm::ivec3(x, y - 1, z));
						TextureIDs.push_back(BlockTypes::blocks[blockID].Top);
						chunk->Quads.push_back({ 0, (CHUNK_WIDTH * CHUNK_HEIGHT * z) + (CHUNK_WIDTH * (y - 1)) + (x) });
					}
					if (chunk->GetBlock(glm::ivec3(x, y, z + 1)) != 0) {
						QuadDirections.push_back((int*)&FrontQuad);
						ipos.push_back(glm::ivec3(x, y, z + 1));
						normal.push_back(glm::vec3(0, 0, -1));
						int blockID = chunk->GetBlock(glm::ivec3(x, y, z + 1));
						TextureIDs.push_back(BlockTypes::blocks[blockID].Back);
						chunk->Quads.push_back({ 5, (CHUNK_WIDTH * CHUNK_HEIGHT * (z + 1)) + (CHUNK_WIDTH * (y)) + (x) });

					}
					if (chunk->GetBlock(glm::ivec3(x, y, z - 1)) != 0) {
						QuadDirections.push_back((int*)&BackQuad);
						ipos.push_back(glm::ivec3(x, y, z - 1));
						normal.push_back(glm::vec3(0, 0, 1));
						int blockID = chunk->GetBlock(glm::ivec3(x, y, z - 1));
						TextureIDs.push_back(BlockTypes::blocks[blockID].Front);
						chunk->Quads.push_back({ 4, (CHUNK_WIDTH * CHUNK_HEIGHT * (z - 1)) + (CHUNK_WIDTH * (y)) + (x) });
					}

					for (int Dir = 0; Dir < QuadDirections.size(); Dir++) {


						//first draw the first tri
						for (int i = 0; i < 6; i++) {

							int index = i;

							switch (i) {
							case 3:
								index = 2;
								break;
							case 4:
								index = 3;
								break;
							case 5:
								index = 0;
								break;
							}


							//first push the vertex positions
							VertexData.push_back(positions[QuadDirections[Dir][index] - 1].x + ipos[Dir].x);
							VertexData.push_back(positions[QuadDirections[Dir][index] - 1].y + ipos[Dir].y);
							VertexData.push_back(positions[QuadDirections[Dir][index] - 1].z + ipos[Dir].z);


							//then push the normal
							VertexData.push_back(normal[Dir].x);
							VertexData.push_back(normal[Dir].y);
							VertexData.push_back(normal[Dir].z);

							int left = TextureIDs[Dir];

							//first get the to index
							float yTex = left / 16;
							float xTex = left % 16;

							float sixth = 1.0f / 16.0f;

							xTex = sixth * xTex;
							yTex = sixth * yTex;

							VertexData.push_back(TexCords[index * 2] / 16.0f + xTex);
							VertexData.push_back(TexCords[index * 2 + 1] / 16.0f + yTex);
						}
					}
						
				}
			}
		}
	}
	//after generating mesh data upload it to the gpu
	chunk->mesh.Reference = Renderer::UploadRawMesh(&VertexData[0], VertexData.size() / 8);
	

}

void ChunkGenerator::GenerateStoneNoise(Chunk* chunk)
{
	for (int y = 0; y < CHUNK_HEIGHT; y++) {
		for (int x = 0; x < CHUNK_WIDTH; x++) {
			for (int z = 0; z < CHUNK_DEPTH; z++) {
				if (y < 95) {
					chunk->SetBlock(1, glm::ivec3(x, y, z));
				}
				else if (y < 99) {
					chunk->SetBlock(3, glm::ivec3(x, y, z));
				}
				else if (y < 100) {
					chunk->SetBlock(2, glm::ivec3(x, y, z));
				}
				
			}
		}
	}
}
