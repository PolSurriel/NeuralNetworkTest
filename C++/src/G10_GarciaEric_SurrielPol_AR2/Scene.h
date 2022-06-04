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
#include "InputManager.h"


enum class SceneState
{
	Running,
	Exit,
	ToPlay,
	ToRanking,
	StartGame,
	Paused,
	GameOver,
	ToMenu
};

class Scene
{
public:
	SceneState state;
	virtual void Update(const InputManager& input) = 0;
	virtual void Draw() = 0;

	~Scene();
};



/*
 *El programador puede crear su propia escena de juego de manera muy sencilla.
 *
 *Solo debe:
 *	1) Crear una clase que herede de play
 *	2) Implementar el metodo constructor e Init para instanciar los gameObjects y canvas de la escena
 *	3) Implementar el metodo OnExit para hacer los deletes de los objetos que no quiera
 *	   conservar al cambiar de escena.
 *	4) Implementar el metodo Update para gestionar el cambio de estados de la escena.
 *	5) Modificar la clase HUD para que el HUD del videojuego sea como quiere.
 *
 */

class Play : public Scene
{

public:
	Play();

	HUD hud;
	std::set<Canvas*> activeCanvas;
	
	void RunningUpdate(const InputManager &input);
	void Draw() override;
	
	virtual void Init() = 0;
	virtual void OnExit() = 0;

};


class TagPlay : public Play
{

public:
	std::list<GameObject*> walls;
	std::list<PowerUp*> powerUps;
	Escapist *firstPlayer;
	Player *secondPlayer;
	EnemySpawnManager * enemySpawnManager;
	Canvas *startCanvas;
	ButtonCanvas *pauseCanvas;
	Button soundButton;

	// sprites
	Sprite* canvasSprite;

	Sprite* firstPlayerSprite;
	Sprite* secondPlayerSprite;
	Sprite* enemySprite;

	Sprite* wallSprite;
	Sprite* powerUpSprite;
	Sprite* hudSprite;
	
	Sprite* cherrySprite;
	Sprite* strawberrySprite;
	Sprite* orangeSprite;


	TagPlay(std::list<GameObject*> _walls, std::list<PowerUp*> _powerUps, Escapist* _firstPlayer, Player* _secondPlayer, EnemySpawnManager* _enemySpawnManager);

	void Init() override;
	void OnExit() override;
	void Update(const InputManager& input) override;

	~TagPlay();
	
};

class Ranking : public Scene
{
	PlayerInfo players[5];
	Button menuButton;

public:
	Ranking();
	void Update(const InputManager &input);
	void Draw();
};

class Menu : public Scene
{
	Button* options[BUTTON_MENU_COUNT];
	Font buttonFont;

public:
	Menu();
	void Update(const InputManager &input);
	void Draw();
	~Menu();
};

class Splash : public Scene
{
	float const ANIMATION_SECONDS_DURATION{3};

	float secondsOnSplashScreen;

	Font titleFont;
	Font subtitleFont;

	Text titleText;
	Text subtitleText;

	Rect titleRect;
	Rect subtitleRect;

public:
	Splash();
	void Update(const InputManager &input);
	void Draw();
	
};