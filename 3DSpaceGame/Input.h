#pragma once
#include <GLFW/glfw3.h>



enum KeyState {
	RELEASED, ONRELEASE, ONPRESS, PRESSED
};
class Input
{
public:

	
	static bool Pressed(int Key);
	static bool OnPressed(int Key);
	static bool Released(int Key);
	static bool OnRelease(int Key);

	static bool MouseButtonPressed(int Key);
	static bool MouseButtonOnPressed(int Key);
	static bool MouseButtonReleased(int Key);
	static bool MouseButtonOnRelease(int Key);
	static float GetMouseYAxis();
	static float GetMouseXAxis();

	static bool ShouldCloseWindow();

	//updates key information like if its pressed or released
	static void Update();
private:
	

	static double LastMouseX;
	static double LastMouseY;

	static double MouseSpeedX;
	static double MouseSpeedY;
	//state of every key  

	static KeyState MouseButtonState[4];

	static KeyState KeyStates[512];
	static bool WindowSelected;

	


};

