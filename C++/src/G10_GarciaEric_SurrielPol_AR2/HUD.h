#pragma once
#include "Types.h"
#include "Fruit.h"

class HUD : public GameObject
{
public:
	Rect screenPosition;
	Font scoreFont;
	Font fruitFont;

	// sprite
	Sprite* backgroundSprite;
	Sprite* cherrySprite;
	Sprite* strawberrySprite;
	Sprite* orangeSprite;
	Sprite* playerSprite;

	// sprites rects
	Rect backgroundSpriteRect;
	Rect cherrySpriteRect;
	Rect strawberrySpriteRect;
	Rect orangeSpriteRect;
	Rect playerSpriteRect;

	// dynamic texts
	Text scoreText;
	Text cherryText;
	Text strawberryText;
	Text orangeText;

	// texts rects
	Rect scoreTextRect;
	Rect cherryTextRect;
	Rect strawberryTextRect;
	Rect orangeTextRect;

	Escapist * mainPlayer;
	
	int fruitCount[(int)FruitType::COUNT];
	
	int score;
	int lives;

	HUD();
	HUD(Escapist * _mainPlayer);
	void InitFonts();
	void InitStats();
	void InitSpritesRects();
	void InitTexts();
	void InitTextRectPositions();
	void UpdateText();
	void UpdateTextRects();
	void UpdateTextRectSizes();
	void SetSprites(Sprite* &_backgroundSprite, Sprite* &_cherrySprite, Sprite* &_strawberrySprite, Sprite* &_orangeSprite, Sprite* &_playerSprite);
	void AddFruit(const FruitType &fruitType, const int &count);
	void Draw();

	void Update(const InputManager& input) override;
	void HandleEvent(std::pair<Event, stream_buff::buffer> evt_info) override;


};
