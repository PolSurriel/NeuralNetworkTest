#pragma once
#include <chrono>

class FPSTime {
private:
	static std::chrono::high_resolution_clock::time_point chronoTime;
	static double secondsPlayed;
	static bool updateFrame;

	static double millisecondsToUpdateFrame;
	static double secondsUntilNow;
	static double millisecondsThisFrame;
	
	// to debug
	static bool printFPS;
	static int frames;

public:

	static double deltaTimeInMilliseconds;

	static void Init();
	static void Update();
	static bool HasToUpdateFrame();

};