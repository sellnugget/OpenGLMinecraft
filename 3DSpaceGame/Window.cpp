#include "Window.h"
#include "GameEvent.h"
GLFWwindow* Window::window;
std::string Window::Title;
std::string Window::Metrics;
int Window::StartHei;
int Window::StartWid;
int Window::Width;
bool Window::WindowResized;
int Window::Height;



void Window::Init(int width, int height, std::string name)
{
    StartWid = width;
    StartHei = height;
    Width = width;
    Height = height;
    // Initialize the library 
    if (!glfwInit())
    {
        throw std::exception("Glfw Failed to Initialize");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //create window 
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    Title = name;
    if (!window)
    {
        glfwTerminate();
        throw std::exception("Failed to Create Window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, OnResize);
    int junk;
    GameEvents::Update.Sub([]() {Update(); }, &junk);

}

void Window::Update()
{
   
    glfwSetWindowTitle(window, (Title + Metrics).c_str());
    Metrics = "";
    WindowResized = false;
}

void Window::OnResize(GLFWwindow* window, int width, int height)
{
    Width = width;
    Height = height;
    glViewport(0, 0, width, height);
    WindowResized = true;



}
