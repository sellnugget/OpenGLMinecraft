#include "Block.h"
std::vector<Block> BlockTypes::blocks = {

	//air
	{0,0,0,0,0,0,   1},
	//stone
	{1,1,1,1,1,1,   0},
	//Grass
	{0,2,3,3,3,3,   0},
	//dirt
	{2,2,2,2,2,2,   0},
	//cobble stone
	{16,16,16,16,16, 16,   0},
	//oak plank
	{4,4,4,4,4,4,   0},
	//Oak log
	{21,21,20,20,20,20,    0},
	//oak leaf
	{52,52,52,52,52,52,    1},
};