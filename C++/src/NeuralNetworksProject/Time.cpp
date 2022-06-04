#include "Time.h"
#include "Consts.h"

void FPSTime::Init() {
	secondsPlayed = 0;
	chronoTime = std::chrono::high_resolution_clock::now();
	millisecondsToUpdateFrame = 1000.0 / FPS;
	millisecondsThisFrame = 0;
	updateFrame = false;
	
	printFPS = PRINT_FPS;
	frames = 0;
	if (printFPS)
	std::cout << "ms: " << millisecondsToUpdateFrame << std::endl;
}

void FPSTime::Update() {
	std::chrono::high_resolution_clock::time_point timeNow = std::chrono::high_resolution_clock::now();

	int s = secondsPlayed;

	secondsUntilNow = std::chrono::duration_cast<std::chrono::duration<double>>(timeNow - FPSTime::chronoTime).count();
	secondsPlayed += secondsUntilNow;

	chronoTime = timeNow;

	// CHECK IF DRAG WINDOW
	if (secondsUntilNow * 1000 > millisecondsToUpdateFrame) {
		secondsUntilNow = millisecondsToUpdateFrame / 1000.0;
	}

	if (millisecondsThisFrame >= millisecondsToUpdateFrame) {
		if (printFPS) {
			std::cout << "." << std::flush;
			frames++;
		}
		deltaTimeInMilliseconds = millisecondsThisFrame;
		millisecondsThisFrame = millisecondsThisFrame - millisecondsToUpdateFrame;
		updateFrame = true;
	}
	else {
		updateFrame = false;
		millisecondsThisFrame += secondsUntilNow * 1000;
	}

	if (printFPS && s != (int)secondsPlayed) {
		std::cout << frames << std::endl;
		frames = 0;
	}
}

bool FPSTime::HasToUpdateFrame() {
	return updateFrame;
}

double FPSTime::secondsPlayed;
std::chrono::high_resolution_clock::time_point FPSTime::chronoTime;
double FPSTime::millisecondsToUpdateFrame;
double FPSTime::millisecondsThisFrame;
int FPSTime::frames;
bool FPSTime::updateFrame;
bool FPSTime::printFPS;
double FPSTime::secondsUntilNow;
double FPSTime::deltaTimeInMilliseconds;
