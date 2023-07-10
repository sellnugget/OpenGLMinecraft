#pragma once
#include <GL/glew.h>
class Texture3D
{
public:

	void LoadData(void* Pixels, int width, int height, int depth, int Format, int DataType, int slot = 0);
	void Bind(int bindslot = 0);
	void Delete();
private:
	unsigned int InstanceID;

};


