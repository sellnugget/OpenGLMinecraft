#pragma once

#include "GLheaders.h"
//Vertex Buffer Objects
namespace GL {

	class VBO
	{
	public:

		unsigned int InstanceID;
		void Gen();
		
		static VBO* Bound;

		bool isBound();
		void BufferData(unsigned int size, void* data, int GpuMemoryLocation);
		void Bind();
	};

}

