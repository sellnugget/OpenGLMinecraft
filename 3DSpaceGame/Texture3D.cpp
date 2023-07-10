#include "Texture3D.h"

void Texture3D::LoadData(void* Pixels, int width, int height, int depth, int Format, int DataType, int slot)
{
	glGenTextures(1, &InstanceID);
	Bind(slot);
	glTexImage2D(GL_TEXTURE_3D, 0, Format, width, height, depth, Format, DataType, Pixels);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
}

void Texture3D::Bind(int bindslot)
{
	glActiveTexture(GL_TEXTURE0 + bindslot);
	glBindTexture(GL_TEXTURE_3D, InstanceID);
}

void Texture3D::Delete()
{
	glDeleteTextures(1, &InstanceID);
}
