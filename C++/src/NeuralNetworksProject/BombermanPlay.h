#pragma once
#include <list>
#include <chrono>
#include <set>

#include "Types.h"
#include "GameObject.h"
#include "HUD.h"
#include "PowerUp.h"
#include "Button.h"
#include "Player.h"
#include "Enemy.h"
#include "Fruit.h"
#include "Canvas.h"
#include "PlayerInfo.h"
#include "Scene.h"
#include "BombermanPlayer.h"
#include "InputManager.h"



class BombermanPlay : public Play
{

public:
	std::list<GameObject*> walls;
	std::list<PowerUp*> powerUps;
	BombermanPlayer* player;
	Canvas* startCanvas;
	ButtonCanvas* pauseCanvas;
	Button soundButton;

	// sprites
	Sprite* canvasSprite;

	Sprite* playerSprite;
	Sprite* enemySprite;

	Sprite* wallSprite;
	Sprite* powerUpSprite;
	Sprite* hudSprite;

	Sprite* cherrySprite;
	Sprite* strawberrySprite;
	Sprite* orangeSprite;


	BombermanPlay(std::list<GameObject*> _walls, std::list<PowerUp*> _powerUps, BombermanPlayer* _player);

	void Init() override;
	void OnExit() override;
	void Update(const InputManager& input) override;

	~BombermanPlay();

};