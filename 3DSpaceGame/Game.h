#pragma once
#include "Renderer.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"
#include "Input.h"
#include "Time.h"
#include "AssetLoader.h"
#include "Physics.h"
class Game
{

private:
public:
	void Run();
	void Start();
	void Update();
	void OnGUI();
};

