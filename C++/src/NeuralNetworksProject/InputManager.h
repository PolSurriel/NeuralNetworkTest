#pragma once
#include <SDL.h>
#include "Types.h"


struct InputManager
{

public:
	
	void UpdateInput() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

			case SDL_WINDOWEVENT:

				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
					closeWindow = true;

					break;

				default:
					break;
				}

				break;

			case SDL_MOUSEMOTION:
				mousePosition.y = event.motion.y;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_w) SetKeyValue(InputManager::Keys::W, true);
				if (event.key.keysym.sym == SDLK_s) SetKeyValue(InputManager::Keys::S, true);
				if (event.key.keysym.sym == SDLK_a) SetKeyValue(InputManager::Keys::A, true);
				if (event.key.keysym.sym == SDLK_d) SetKeyValue(InputManager::Keys::D, true);
				if (event.key.keysym.sym == SDLK_UP) SetKeyValue(InputManager::Keys::UP_ARROW, true);
				if (event.key.keysym.sym == SDLK_DOWN) SetKeyValue(InputManager::Keys::DOWN_ARROW, true);
				if (event.key.keysym.sym == SDLK_LEFT) SetKeyValue(InputManager::Keys::LEFT_ARROW, true);
				if (event.key.keysym.sym == SDLK_RIGHT) SetKeyValue(InputManager::Keys::RIGHT_ARROW, true);
				if (event.key.keysym.sym == SDLK_SPACE) SetKeyValue(InputManager::Keys::SPACE, true);
				if (event.key.keysym.sym == SDLK_p) SetKeyValue(InputManager::Keys::P, true);
				if (event.key.keysym.sym == SDLK_ESCAPE) { 
					SetKeyValue(InputManager::Keys::ESC, true); 
				};
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_w) SetKeyValue(InputManager::Keys::W, false);
				if (event.key.keysym.sym == SDLK_s) SetKeyValue(InputManager::Keys::S, false);
				if (event.key.keysym.sym == SDLK_a) SetKeyValue(InputManager::Keys::A, false);
				if (event.key.keysym.sym == SDLK_d) SetKeyValue(InputManager::Keys::D, false);
				if (event.key.keysym.sym == SDLK_UP) SetKeyValue(InputManager::Keys::UP_ARROW, false);
				if (event.key.keysym.sym == SDLK_DOWN) SetKeyValue(InputManager::Keys::DOWN_ARROW, false);
				if (event.key.keysym.sym == SDLK_LEFT) SetKeyValue(InputManager::Keys::LEFT_ARROW, false);
				if (event.key.keysym.sym == SDLK_RIGHT) SetKeyValue(InputManager::Keys::RIGHT_ARROW, false);
				if (event.key.keysym.sym == SDLK_SPACE) SetKeyValue(InputManager::Keys::SPACE, false);
				if (event.key.keysym.sym == SDLK_p) SetKeyValue(InputManager::Keys::P, false);
				if (event.key.keysym.sym == SDLK_ESCAPE) SetKeyValue(InputManager::Keys::ESC, false);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) SetKeyValue(InputManager::Keys::MOUSE_LEFT, true);
				if (event.button.button == SDL_BUTTON_RIGHT) SetKeyValue(InputManager::Keys::MOUSE_RIGHT, true);
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) SetKeyValue(InputManager::Keys::MOUSE_LEFT, false);
				if (event.button.button == SDL_BUTTON_RIGHT) SetKeyValue(InputManager::Keys::MOUSE_RIGHT, false);
				break;
			}
		}
	}



	enum class Keys {
		NONE, W, S, A, D, UP_ARROW, DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, SPACE, P, ESC, MOUSE_LEFT, MOUSE_RIGHT, COUNT
	};

	bool closeWindow;
	bool keysDown[(int)Keys::COUNT];
	bool keysPressed[(int)Keys::COUNT];

	Vector2 mousePosition;

	void SetKeyValue(Keys key, bool pressed) {

		if (!keysPressed[(int)key]) {
			keysDown[(int)key] = pressed;
		}

		keysPressed[(int)key] = pressed;

		/*if (!keysPressed[(int)key])
			keysDown[(int)key] = keysPressed[(int)key] = pressed;
		else
			keysDown[(int)key] = keysPressed[(int)key] = false;*/
	}

	void SetFalseKeyDown() {
		for (int i = 0; i < (int)Keys::COUNT; i++) {
			keysDown[i] = false;
		}
	}

	InputManager() {
		SetFalseKeyDown();
		closeWindow = false;
	}


};