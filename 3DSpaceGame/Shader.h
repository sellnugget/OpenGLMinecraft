#pragma once
#include "GLheaders.h"
namespace GL {
	class Shader
	{
	public:

		unsigned int InstanceID;
		std::string VertSourceBuffer;
		std::string FragSourceBuffer;
		int ShaderCounts;

		//compiles both vertex and fragment shaders
		void CompileShader(const char* VertexPath, const char* FragPath);

		//loads these shaders into the vert and frag source buffer
		void LoadMainSource(const char* VertexPath, const char* FragPath);

		//these will insert a shader with a main function along with a call to it.
		void AddVertShader(const char* VertPath);
		void AddFragShader(const char* FragPath);

		void CompileBuffers();

		//activates this shader
		void Use();
		//sets uniform in shader of type float
		void setUniform(const std::string& name, float value);
		//sets uniform in shader of type int
		void setUniform(const std::string& name, int value);
		//sets uniform in shader of type bool
		void setUniform(const std::string& name, bool value);
		//set uniform in shader of vec2
		void setUniform(const std::string& name, glm::vec2 value);
		//set uniform in shader of vec3
		void setUniform(const std::string& name, glm::vec3 value);
		//set uniform in shader of vec4
		void setUniform(const std::string& name, glm::vec4 value);
		
		//set uniform in shader of mat4
		void setUniform(const std::string& name, glm::mat4 value);
	};

}

