#pragma once
class Time
{
public:

	//returns the time since last frame. 
	static float GetDeltaTime();
	//returns time since last scene was loaded.
	static float GetElapsedTime();
	static void Update();

private:

	static float DeltaTime;
	static float ElapsedTime;

};

