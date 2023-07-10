#include "Renderer.h"
#include "Input.h"
#include "Time.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

std::map<int, GPUMODEL> Renderer::models;
int Renderer::newInstanceCount;
int Renderer::samples = 8;
GL::Shader Renderer::ProcessShader;
GL::Shader Renderer::UIShader;
unsigned int Renderer::framebufferTexture;
unsigned int Renderer::DepthbufferTexture;
unsigned int Renderer::FBO;
unsigned int Renderer::RBO;

unsigned int Renderer::quadVAO, Renderer::quadVBO;

void Renderer::Init()
{

	float rectangleVertices[] =
	{
		// Coords    // texCoords
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	RegenFrameBuffer();
	
	glfwSwapInterval(0);
	
	ProcessShader.CompileShader("Assets\\Shaders\\ScreenVert.glsl", "Assets\\Shaders\\ScreenFrag.glsl");
	UIShader.CompileShader("Assets\\Shaders\\UIVertex.glsl", "Assets\\Shaders\\UIFragment.glsl");
}



void Renderer::Update()
{
	if (Window::WindowResized) {
		glDeleteFramebuffers(1, &FBO);
		glDeleteRenderbuffers(1, &RBO);
		glDeleteTextures(1, &framebufferTexture);
		RegenFrameBuffer();
	}

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// first pass

	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	


}
float blur = 0.001f;
float BlurSamples = 1;
int layers = 1;
float Threshold;
bool DistancedBlur;
void Renderer::PostProcess()
{
	//set Blur
	ProcessShader.setUniform("screenTexture", 0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ProcessShader.Use();
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);


}



int Renderer::LoadModelGpu(Model& model)
{
	GPUMODEL GpuModel;


	GpuModel.Triangles = model.VertIndices.size();
	GpuModel.vao.Gen();
	GpuModel.vbo.Gen();
	GpuModel.vao.Bind();
	GpuModel.vbo.Bind();
	GpuModel.vbo.BufferData(model.VertIndices.size() * sizeof(Vertex), model.GetMeshData(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	models[newInstanceCount] = GpuModel;
	newInstanceCount++;
	return newInstanceCount - 1;
}
int Renderer::UploadRawMesh(float* Data, int size)
{
	GPUMODEL GpuModel;

	GpuModel.Triangles = size;
	GpuModel.vao.Gen();
	GpuModel.vbo.Gen();
	GpuModel.vao.Bind();
	GpuModel.vbo.Bind();

	GpuModel.vbo.BufferData(size * sizeof(Vertex), Data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	models[newInstanceCount] = GpuModel;
	newInstanceCount++;
	return newInstanceCount - 1;
}

void Renderer::DeleteMesh(int Reference)
{
	if (models.count(Reference)) {
		glDeleteBuffers(1, &models[Reference].vao.InstanceID);
		glDeleteVertexArrays(1, &models[Reference].vbo.InstanceID);
		models.erase(Reference);
	}
	
}

void Renderer::DrawUI(glm::vec2 position, glm::vec2 Scale, glm::vec2 TexturePosition, glm::vec2 TextureScale)
{
	UIShader.Use();
	UIShader.setUniform("Position", position);

	Scale *= TextureScale;

	float aspect = ((float)Window::Width) / Window::Height;

	Scale.x /= aspect;
	
	UIShader.setUniform("Scale", Scale);
	UIShader.setUniform("TextureScale", TextureScale);
	UIShader.setUniform("TextureOffset", TexturePosition);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}



void Renderer::DrawMesh(const Mesh& mesh)
{
	mesh.shader->Use();
	models[mesh.Reference].vao.Bind();

	//move the object
	glm::mat4 Transform = glm::mat4(1.0f);

	Transform = glm::translate(Transform, mesh.Position);
	Transform = glm::scale(Transform, mesh.Scale);
	Transform = glm::rotate(Transform, glm::radians(mesh.Rotation.x), glm::vec3(0, 1, 0));
	Transform = glm::rotate(Transform, glm::radians(mesh.Rotation.y), glm::vec3(1, 0, 0));
	Transform = glm::rotate(Transform, glm::radians(mesh.Rotation.z), glm::vec3(0, 0, 1));
	//Transform = glm::translate(Transform, mesh.Origin);
	mesh.shader->setUniform("Model", Transform);


	glDrawArrays(GL_TRIANGLES, 0, models[mesh.Reference].Triangles);
}


void Renderer::RegenFrameBuffer()
{

	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &framebufferTexture);
	glDeleteTextures(1, &DepthbufferTexture);
	//first create frame buffer object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::Width, Window::Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);


	glGenTextures(1, &DepthbufferTexture);
	glBindTexture(GL_TEXTURE_2D, DepthbufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::Width, Window::Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthbufferTexture, 0);


	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::Width, Window::Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;



}

void Renderer::DrawInstancedMesh(const Mesh& mesh, int Count, bool Translate = true)
{
	mesh.shader->Use();
	models[mesh.Reference].vao.Bind();


	if (Translate) {
		//move the object
		glm::mat4 Transform = glm::mat4(1.0f);

		Transform = glm::translate(Transform, mesh.Position);
		Transform = glm::scale(Transform, mesh.Scale);
		Transform = glm::rotate(Transform, glm::radians(mesh.Rotation.x), glm::vec3(0, 1, 0));
		Transform = glm::rotate(Transform, glm::radians(mesh.Rotation.y), glm::vec3(1, 0, 0));
		Transform = glm::rotate(Transform, glm::radians(mesh.Rotation.z), glm::vec3(0, 0, 1));
		//Transform = glm::translate(Transform, mesh.Origin);
		mesh.shader->setUniform("Model", Transform);
	}
	else {
		mesh.shader->setUniform("Model", glm::mat4(1.0f));
	}
	

	glDrawArraysInstanced(GL_TRIANGLES, 0, models[mesh.Reference].Triangles, Count);
}
