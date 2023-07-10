#include "Game.h"
#include "Camera.h"
#include "GameEvent.h"
#include "ChunkGenerator.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Texture2D.h"
void Game::Run()
{
	Window::Init(640, 480, "Game");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Window::window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	if (glewInit() != GLEW_OK) {
		std::cout << "Fail";
	};


	Asset::InitAssetHandler();
	Renderer::Init();


	Start();

	

	while (!Input::ShouldCloseWindow()) {



		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Time::Update();
		Renderer::Update();
		
		Update();

		Renderer::PostProcess();

		OnGUI();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(Window::window);
		Input::Update();




	}
}

GL::Shader DefaultShader;
Model cube;
Model SpaceShip;
Model Quad;
int ModelGpu;
Mesh Cube;
Mesh ShipMesh;
Camera* camera;
Texture2D BlockTextures;
Texture2D Widgets;
Chunk* chunk;


glm::vec3 LastPosition;

void Game::Start()
{
	camera = new Camera();

	camera->Position = glm::vec3(8, 102, 8);
	LastPosition = camera->Position;
	//loads the models into memory
	cube.LoadObjModel("Assets\\Models\\Cube.obj");
	SpaceShip.LoadObjModel("Assets\\Models\\MySpaceShip.obj");
	Quad.LoadObjModel("Assets\\Models\\Quad.obj");


	//creates a gpu reference of the model
	Cube.Reference = Renderer::LoadModelGpu(cube);

	//compiles all shaders
	DefaultShader.LoadMainSource("Assets\\Shaders\\DefaultVertex.glsl", "Assets\\Shaders\\DefaultFrag.glsl");

	DefaultShader.AddVertShader("Assets\\Shaders\\Star_Vert.glsl");

	DefaultShader.CompileBuffers();


	//set references
	ShipMesh.shader = &DefaultShader;
	ShipMesh.Reference = Renderer::LoadModelGpu(SpaceShip);

	Cube.shader = &DefaultShader;
	camera->shader = &DefaultShader;

	//set the texture default
	DefaultShader.setUniform("texture1", 0);

	//con
	Image blockassets;
	blockassets.LoadFromAsset("Assets\\Textures\\terrain.png", GL_RGBA);
	BlockTextures.LoadFromImage(&blockassets);



	Image ImageWidgets;
	ImageWidgets.LoadFromAsset("Assets\\Textures\\widgets.png", GL_RGBA);
	Widgets.LoadFromImage(&ImageWidgets);



	chunk = new Chunk();
	chunk->mesh.shader = &DefaultShader;
	chunk->mesh.Position = glm::vec3(0, 0, 0);
	ChunkGenerator::GenerateStoneNoise(chunk);
	ChunkGenerator::CreateMesh(chunk);
}
float xmul = 0;
float ymul = 0;

int block = 1;

glm::vec3 LightDirection = glm::vec3(0,1,0);
float AmbientLight = 0;

float energyloss = 0.5;
int bounces = 1;

void Game::Update()
{


	GameEvents::Update.Call();

	//handle block placing

	RayHit Payload = RayChunkCast(chunk, glm::normalize(camera->cameraFront), camera->Position, 8);


	char key = '0';
	for (int i = 0; i < 10; i++) {
		if (Input::OnPressed(key + i)) {
			block = i;
		}
	}
	if (Payload.Distance != RAY_MISSED) {

		Cube.Position = Payload.HitPoint;
		Cube.Scale = glm::vec3(1, 1, 1);
		BlockTextures.Bind();
		//Renderer::DrawMesh(Cube);
		if (Input::MouseButtonOnPressed(1)) {



			if (Payload.Distance != RAY_MISSED) {
				//temp

				if (chunk->GetBlock(Payload.BlockHit + (glm::ivec3)Payload.Normal) != 0) {
					std::cout << "ERROR: bad ray cast";
				}
				else {
					chunk->SetBlock(block, Payload.BlockHit + (glm::ivec3)Payload.Normal);
					ChunkGenerator::CreateMesh(chunk);
				}


			}


		}
		if (Input::MouseButtonOnPressed(0)) {
			chunk->SetBlock(0, Payload.BlockHit);
			ChunkGenerator::CreateMesh(chunk);
		}
	}




	BlockTextures.Bind();
	chunk->ChunkTexture.Bind(1);
	Renderer::DrawMesh(chunk->mesh);

	//handle collision system
	float offset = 2;
	RayHit ray;
	//camera->Position -= glm::vec3(0, 1, 0) * Time::GetDeltaTime();
	ray = RayChunkCast(chunk, glm::normalize((LastPosition - glm::vec3(0, offset, 0)) - (camera->Position - glm::vec3(0, offset, 0))), LastPosition - glm::vec3(0, offset, 0), 8);
	if (ray.Distance != RAY_MISSED) {
		//camera->Position = ray.HitPoint + ray.Normal * 0.001f  + glm::vec3(0, offset, 0);
	}



	//Blur Level Controller
	ImGui::Begin("Settings");
	ImGui::DragFloat3("Player Position", (float*)&camera->Position, 0.01);
	ImGui::DragFloat3("Player Direction", (float*)&camera->cameraFront, 1);
	ImGui::DragFloat3("LightDirection", (float*)&LightDirection, 0.01);
	ImGui::DragFloat("AmbientLight", &AmbientLight, 0.01f);
	ImGui::DragFloat("EnergyLoss", &energyloss, 0.01f);
	ImGui::DragInt("Bounces", &bounces);
	ImGui::End();


	if (!(LightDirection.x == 0 && LightDirection.y == 0 && LightDirection.z == 0)) {
		//LightDirection = glm::normalize(LightDirection);
	}
	DefaultShader.setUniform("LightDirection", LightDirection);
	DefaultShader.setUniform("AmbientLight", AmbientLight);
	DefaultShader.setUniform("CameraPosition", camera->Position);
	DefaultShader.setUniform("EnergyLoss", energyloss);
	DefaultShader.setUniform("bounces", bounces);








	LastPosition = camera->Position;
}



void Game::OnGUI()
{
	float UIScaler = 1;
	Widgets.Bind();

	//draw block cursor
	Renderer::DrawUI(glm::vec2(0, 0), glm::vec2(1, 1) * UIScaler, glm::vec2(1 - 1/16.0, 0), glm::vec2(1 / 16.0, 1 / 16.0));


	//draw hot bar
	Renderer::DrawUI(glm::vec2(0, -1 + 1/8.0), glm::vec2(1, 1) * UIScaler, glm::vec2(0, 0), glm::vec2(0.75, 1/11.75));

	//draw held block
	BlockTextures.Bind();
	Renderer::DrawUI(glm::vec2(0, -1 + 1 / 8.0), glm::vec2(1, 1) * UIScaler, glm::vec2(0, 0), glm::vec2(0.75, 1 / 11.75));
}





