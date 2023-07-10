#pragma once
//Vertex Array Objects

#include "GLheaders.h"
namespace GL {
	class VAO
	{
	public:
		unsigned int InstanceID;
		static VAO* Bound;
		bool isBound();

		void Bind();

		void Gen();


	};

}


