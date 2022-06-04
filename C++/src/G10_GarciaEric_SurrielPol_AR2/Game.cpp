#include "Game.h"
#include "Scene.h"
#include "PlayableCharacter.h"
#include "Consts.h"
#include "Renderer.h"
#include "AudioManager.h"
#include "GameData.h"

#include <list>
#include <iostream>
#include <chrono>

Game::Game() {}


void Game::GameLoop() {

	state = GameState::SplashScreen;
	scene = new Splash;
	Renderer* rend = Renderer::Instance();
	rend->SetRenderDrawColor(0, 0, 0);

	std::list<GameObject*> walls;
	std::list<PowerUp*> powerUps;
	Escapist *firstPlayer;
	Player *secondPlayer;
	EnemySpawnManager *enemySpawnManager;
	
	BombermanPlayer* __player;
	
	FPSTime::Init();
	while (scene->state != SceneState::Exit) {
		FPSTime::Update();
		if (!FPSTime::HasToUpdateFrame()) continue;

		// only if updateFrame:
		//       |
		//       v

		UpdateInput();
		
		switch (scene->state)
		{
		case SceneState::ToMenu:
			if (state == GameState::Play) {
				((BombermanPlay*)scene)->OnExit();
			}
			delete scene;
			scene = new Menu;
			state = GameState::Menu;
			break;
		case SceneState::ToPlay:
			delete scene;

			walls = std::list<GameObject*>();
			powerUps = std::list<PowerUp*>();

			firstPlayer = new Escapist;
			secondPlayer = new Player;

			GameData::ImportFromXMLConfigFile(XML_CONFIG_FILE, walls, powerUps, firstPlayer, secondPlayer, enemySpawnManager);
			firstPlayer->SetPlayableCharacterDirectionalKeys((int)InputManager::Keys::W, (int)InputManager::Keys::S, (int)InputManager::Keys::A, (int)InputManager::Keys::D);
			secondPlayer->SetPlayableCharacterDirectionalKeys((int)InputManager::Keys::UP_ARROW, (int)InputManager::Keys::DOWN_ARROW, (int)InputManager::Keys::LEFT_ARROW, (int)InputManager::Keys::RIGHT_ARROW);

			__player = new BombermanPlayer();
			__player->position = firstPlayer->playableCharacters[0]->position;
			__player->SetDirectionalKeys((int)InputManager::Keys::W, (int)InputManager::Keys::S, (int)InputManager::Keys::A, (int)InputManager::Keys::D);
			

			scene = new BombermanPlay(walls, powerUps, __player);
			state = GameState::Play;
			break;
		case SceneState::ToRanking:
			delete scene;
			scene = new Ranking;
			state = GameState::Ranking;
			break;
		}
		
		scene->Update(inputManager);

		rend->Clear();
		scene->Draw();
		rend->Render();

	}

	
	delete scene;
	delete rend;

	
}

void Game::UpdateInput() {

	inputManager.SetFalseKeyDown();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			inputManager.mousePosition.x = event.motion.x;
			inputManager.mousePosition.y = event.motion.y;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_w) inputManager.SetKeyValue(InputManager::Keys::W, true);
			if (event.key.keysym.sym == SDLK_s) inputManager.SetKeyValue(InputManager::Keys::S, true);
			if (event.key.keysym.sym == SDLK_a) inputManager.SetKeyValue(InputManager::Keys::A, true);
			if (event.key.keysym.sym == SDLK_d) inputManager.SetKeyValue(InputManager::Keys::D, true);
			if (event.key.keysym.sym == SDLK_UP) inputManager.SetKeyValue(InputManager::Keys::UP_ARROW, true);
			if (event.key.keysym.sym == SDLK_DOWN) inputManager.SetKeyValue(InputManager::Keys::DOWN_ARROW, true);
			if (event.key.keysym.sym == SDLK_LEFT) inputManager.SetKeyValue(InputManager::Keys::LEFT_ARROW, true);
			if (event.key.keysym.sym == SDLK_RIGHT) inputManager.SetKeyValue(InputManager::Keys::RIGHT_ARROW, true);
			if (event.key.keysym.sym == SDLK_SPACE) inputManager.SetKeyValue(InputManager::Keys::SPACE, true);
			if (event.key.keysym.sym == SDLK_p) inputManager.SetKeyValue(InputManager::Keys::P, true);
			if (event.key.keysym.sym == SDLK_ESCAPE) inputManager.SetKeyValue(InputManager::Keys::ESC, true);
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_w) inputManager.SetKeyValue(InputManager::Keys::W, false);
			if (event.key.keysym.sym == SDLK_s) inputManager.SetKeyValue(InputManager::Keys::S, false);
			if (event.key.keysym.sym == SDLK_a) inputManager.SetKeyValue(InputManager::Keys::A, false);
			if (event.key.keysym.sym == SDLK_d) inputManager.SetKeyValue(InputManager::Keys::D, false);
			if (event.key.keysym.sym == SDLK_UP) inputManager.SetKeyValue(InputManager::Keys::UP_ARROW, false);
			if (event.key.keysym.sym == SDLK_DOWN) inputManager.SetKeyValue(InputManager::Keys::DOWN_ARROW, false);
			if (event.key.keysym.sym == SDLK_LEFT) inputManager.SetKeyValue(InputManager::Keys::LEFT_ARROW, false);
			if (event.key.keysym.sym == SDLK_RIGHT) inputManager.SetKeyValue(InputManager::Keys::RIGHT_ARROW, false);
			if (event.key.keysym.sym == SDLK_SPACE) inputManager.SetKeyValue(InputManager::Keys::SPACE, false);
			if (event.key.keysym.sym == SDLK_p) inputManager.SetKeyValue(InputManager::Keys::P, false);
			if (event.key.keysym.sym == SDLK_ESCAPE) inputManager.SetKeyValue(InputManager::Keys::ESC, false);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) inputManager.SetKeyValue(InputManager::Keys::MOUSE_LEFT, true);
			if (event.button.button == SDL_BUTTON_RIGHT) inputManager.SetKeyValue(InputManager::Keys::MOUSE_RIGHT, true);
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) inputManager.SetKeyValue(InputManager::Keys::MOUSE_LEFT, false);
			if (event.button.button == SDL_BUTTON_RIGHT) inputManager.SetKeyValue(InputManager::Keys::MOUSE_RIGHT, false);
			break;
		}
	}
}
