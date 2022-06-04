#pragma once
#include "Scene.h"
#include "BombermanPlay.h"
#include "PlayableCharacter.h"
#include "Time.h"
#include "InputManager.h"

enum class GameState
{
	SplashScreen,
	Menu,
	Play,
	Ranking
};

class Game
{
	InputManager inputManager;
	std::string rankingFilePath;
	std::string mapFilePath;
	Scene* scene;

public:

	Game();
	GameState state;


	void GameLoop();
	void UpdateInput();
	
};