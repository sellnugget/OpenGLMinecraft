#include "Image.h"
#include "AssetLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
void Image::LoadFromAsset(std::string Path, int Format)
{
	unsigned char* Data = Asset::LoadEntireAsset(Path);
	int nrChannels;
	

	int GLtoSTBI;

	switch (Format) {
	case GL_R:
		GLtoSTBI = STBI_grey;
		break;
	case GL_RG:
		GLtoSTBI = STBI_grey_alpha;
		break;
	case GL_RGB:
		GLtoSTBI = STBI_rgb;
		break;
	case GL_RGBA:
		GLtoSTBI = STBI_rgb_alpha;
		break;
	}
	

	this->Format = Format;
	this->DataType = GL_UNSIGNED_BYTE;
	Pixels = stbi_load_from_memory(Data, Asset::GetFileSize(Path.c_str()), &Width, &Height, &nrChannels, GLtoSTBI);
	delete[] Data;
}

Image::~Image()
{
	delete[] Pixels;
}
