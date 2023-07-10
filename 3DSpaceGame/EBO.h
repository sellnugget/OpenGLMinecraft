#pragma once
#include "GLheaders.h"
namespace GL {

	class EBO 
	{
	public:
		unsigned int InstanceID;
		static EBO* Bound;

		void Gen();

		bool isBound();
		void BufferData(int size, void* data, int MemoryLocation);
		void Bind();

	};
}


