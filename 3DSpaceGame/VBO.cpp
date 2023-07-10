#include "VBO.h"

namespace GL {

	VBO* VBO::Bound;

	void VBO::Gen()
	{
		glGenBuffers(1, &InstanceID);
	}
	bool VBO::isBound()
	{
		return this == Bound;
	}
	void VBO::BufferData(unsigned int size, void* data, int GpuMemoryLocation)
	{
		glBufferData(GL_ARRAY_BUFFER, size, data, GpuMemoryLocation);
	}
	void VBO::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, InstanceID);
		Bound = this;
	}
}