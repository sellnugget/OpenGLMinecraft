#pragma once
#include "Image.h"
class Texture2D
{
public:

	void Create();
	void LoadData(void* Pixels, int width, int height, int Format, int DataType, int slot = 0);
	void LoadFromImage(Image* image, int slot = 0);
	void Bind(int bindslot = 0);
	void Delete();
private:
	unsigned int InstanceID;

};
