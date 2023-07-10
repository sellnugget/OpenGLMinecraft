#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
class Window
{
public:


	static bool WindowResized;

	static GLFWwindow* window;
	static void Init(int width, int height, std::string name);
	static std::string Title;
	static std::string Metrics;
	static int Width;
	static int Height;
	static int StartWid;
	static int StartHei;
private:

	static void Update();
	static void OnResize(GLFWwindow* window, int width,int height);
};

