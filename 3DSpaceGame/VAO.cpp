#include "VAO.h"


namespace GL {


	VAO* VAO::Bound;

	bool VAO::isBound()
	{
		return this == Bound;
	}

	void VAO::Bind()
	{
		glBindVertexArray(InstanceID);
		Bound = this;
	}
	void VAO::Gen()
	{
		glGenVertexArrays(1, &InstanceID);
	}
}