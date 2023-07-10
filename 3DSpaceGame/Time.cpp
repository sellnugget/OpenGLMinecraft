#include "Time.h"
#include <GLFW/glfw3.h>
#include "Window.h"
#include <string>
#include <iomanip>
#include <sstream>

float Time::DeltaTime;
float Time::ElapsedTime;

float Time::GetDeltaTime()
{
    return DeltaTime;
}

float Time::GetElapsedTime()
{
    return ElapsedTime;
}

void Time::Update()
{
    float CurrentTime = glfwGetTime();
    //simple calculation to get time since last frame
    DeltaTime = CurrentTime - ElapsedTime;
    ElapsedTime = CurrentTime;

    //then we set the fps for the window

    float fps = 1 / DeltaTime;
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << fps;
    Window::Metrics += (" fps " + stream.str());
}
