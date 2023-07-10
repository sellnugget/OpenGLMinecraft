#pragma once
#include <vector>
#include <glm/glm.hpp>
struct Block
{
	//texture ids for sides of block
	int Top;
	int Bottom;
	int Left;
	int Right;
	int Front;
	int Back;


	bool Transparent;
};


class BlockTypes {
public:
	static std::vector<Block> blocks;
};

