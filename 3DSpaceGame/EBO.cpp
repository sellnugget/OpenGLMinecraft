#include "EBO.h"

namespace GL {


	EBO* EBO::Bound;

	void EBO::Gen()
	{
		glGenBuffers(1, &InstanceID);
	}
	bool EBO::isBound()
	{
		return this == Bound;
	}
	void EBO::BufferData(int size, void* data, int MemoryLocation)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, MemoryLocation);
	}
	void EBO::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, InstanceID);
		Bound = this;
	}
}