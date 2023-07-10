#include "Input.h"
#include "Window.h"

KeyState Input::KeyStates[512];
KeyState Input::MouseButtonState[4];
double Input::LastMouseX;
double Input::LastMouseY;
double Input::MouseSpeedX;
double Input::MouseSpeedY;
bool Input::WindowSelected = false;

//checks if the key is currently held down
bool Input::Pressed(int Key)
{
	return KeyStates[Key] == PRESSED;
}

//checks if the key was pressed this frame
bool Input::OnPressed(int Key)
{
	return KeyStates[Key] == ONPRESS;
}
//checks if the key is not being pressed
bool Input::Released(int Key)
{
	return KeyStates[Key] == RELEASED;
}
//checks if the key was released this frame
bool Input::OnRelease(int Key)
{
	return KeyStates[Key] == ONRELEASE;
}

bool Input::MouseButtonPressed(int Key)
{
	return MouseButtonState[Key] == PRESSED;
}

bool Input::MouseButtonOnPressed(int Key)
{
	return MouseButtonState[Key] == ONPRESS;
}

bool Input::MouseButtonReleased(int Key)
{
	return MouseButtonState[Key] == RELEASED;
}

bool Input::MouseButtonOnRelease(int Key)
{
	return MouseButtonState[Key] == ONRELEASE;
}

float Input::GetMouseYAxis()
{
	if (!WindowSelected) {
		return 0.0f;
	}
	return MouseSpeedY;
}

float Input::GetMouseXAxis()
{
	if (!WindowSelected) {
		return 0.0f;
	}
	return MouseSpeedX;
}

bool Input::ShouldCloseWindow()
{
	return glfwWindowShouldClose(Window::window);
}

//updates all key info. DONT CALL UNLESS IN CORE
void Input::Update()
{

	for (int i = 0; i < 512; i++) {
		int value = glfwGetKey(Window::window, i);

		KeyState LastFrame = KeyStates[i];
		if (value == GLFW_RELEASE) {

			if (LastFrame == PRESSED) {
				KeyStates[i] = ONRELEASE;
			}
			else {
				KeyStates[i] = RELEASED;
			}
		}
		else if (value == GLFW_PRESS) {
			
			if (LastFrame == RELEASED) {
				KeyStates[i] = ONPRESS;
			}
			else {
				KeyStates[i] = PRESSED;
			}
		}

	}


	for (int i = 0; i < 4; i++) {
		int value = glfwGetMouseButton(Window::window, i);

		KeyState LastFrame = MouseButtonState[i];
		if (value == GLFW_RELEASE) {

			if (LastFrame == PRESSED) {
				MouseButtonState[i] = ONRELEASE;
			}
			else {
				MouseButtonState[i] = RELEASED;
			}
		}
		else if (value == GLFW_PRESS) {

			if (LastFrame == RELEASED) {
				MouseButtonState[i] = ONPRESS;
			}
			else {
				MouseButtonState[i] = PRESSED;
			}
		}
		if (!WindowSelected) {
			MouseButtonState[i] = RELEASED;
		}
	}
	double NewX;
	double NewY;
	glfwGetCursorPos(Window::window, &NewX, &NewY);
	MouseSpeedX = NewX - LastMouseX;
	MouseSpeedY = NewY - LastMouseY;
	LastMouseX = NewX;
	LastMouseY = NewY;

	if (Input::OnPressed(GLFW_KEY_ESCAPE) && WindowSelected) {
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		WindowSelected = false;
	}
	else if (Input::OnPressed(GLFW_KEY_ESCAPE) && !WindowSelected) {
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		WindowSelected = true;
	}



	

	/* Poll for and process events */
	glfwPollEvents();
}
