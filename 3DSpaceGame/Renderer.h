#pragma once
#include <gl/glew.h>
#include "Window.h"
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "VBO.h"
#include "Shader.h"
#include <vector>
#include <map>
#include "EBO.h"
#include "Mesh.h"
#include "Model.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

struct GPUMODEL {
	GL::VAO vao;
	//buffer data
	GL::VBO vbo;
	int Triangles;
};

class Renderer
{
public:
	static void Init();
	static void Update();
	static void PostProcess();
	static int samples;

	
	//loads a model onto the gpu and returns the models instance id
	static int LoadModelGpu(Model& model);
	static int UploadRawMesh(float* Data, int size);
	static void DrawMesh(const Mesh& mesh);
	static void DrawInstancedMesh(const Mesh& mesh, int Count, bool Translate);
	static void DrawUI(glm::vec2 position, glm::vec2 Scale, glm::vec2 TexturePosition, glm::vec2 TextureScale);
	static void DeleteMesh(int Reference);
private:
	
	static std::map<int, GPUMODEL> models;
	static int newInstanceCount;
	static GL::Shader ProcessShader;
	static GL::Shader UIShader;
	static unsigned int FBO;
	static unsigned int framebufferTexture;
	static unsigned int DepthbufferTexture;
	static unsigned int RBO;

	static unsigned int quadVAO, quadVBO;


	static void RegenFrameBuffer();
};

