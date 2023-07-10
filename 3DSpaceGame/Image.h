#pragma once
#include <inttypes.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>



class Image
{
public:
	//Just a pointer to pixel data that can be represented in any format
	unsigned char* Pixels;

	int Width;
	int Height;

	//just a open gl enum
	int Format = GL_RGBA;
	int DataType = GL_UNSIGNED_BYTE;

	void LoadFromAsset(std::string Path, int Format);


	~Image();
};

