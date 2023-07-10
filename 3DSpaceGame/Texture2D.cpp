#include "Texture2D.h"

void Texture2D::Create()
{

}

void Texture2D::LoadData(void* Pixels, int width, int height, int Format, int DataType, int slot)
{

	glGenTextures(1, &InstanceID);
	Bind(slot);
	glTexImage2D(GL_TEXTURE_2D, 0, Format, width, height, 0, Format, DataType, Pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
}

void Texture2D::LoadFromImage(Image* image, int slot)
{
	glGenTextures(1, &InstanceID); 
	Bind(slot);
	glTexImage2D(GL_TEXTURE_2D, 0, image->Format, image->Width, image->Height, 0, image->Format, image->DataType, image->Pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
}

void Texture2D::Bind(int bind)
{
	glActiveTexture(GL_TEXTURE0 + bind);
	glBindTexture(GL_TEXTURE_2D, InstanceID);
	
}

void Texture2D::Delete()
{
	glDeleteTextures(1, &InstanceID);
}
