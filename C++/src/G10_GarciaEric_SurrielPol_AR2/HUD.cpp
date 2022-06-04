#include <string>

#include "HUD.h"
#include "Consts.h"
#include "Renderer.h"
#include "Fruit.h"

HUD::HUD(Escapist * _mainPlayer) : mainPlayer(_mainPlayer) {
	InitFonts();
	InitStats();
	InitSpritesRects();
}

HUD::HUD()
{
	InitFonts();
	InitStats();
	InitSpritesRects();
}


void HUD::InitFonts() {
	scoreFont = {
		"hudScoreFont",
		GAMEPLAY_FONT_PATH,
		HUD_FONT_SIZE
	};

	fruitFont = {
		"hudFruitFont",
		GAMEPLAY_FONT_PATH,
		HUD_FONT_SIZE
	};

	Renderer* rend = Renderer::Instance();
	rend->LoadFont(scoreFont);
	rend->LoadFont(fruitFont);
}

void HUD::InitStats() {
	for (auto& fruit : fruitCount) {
		fruit = 0;
	}

	score = 0;
	lives = ESCAPIST_LIVES;
}

void HUD::InitSpritesRects() {
	screenPosition = { static_cast<int>(MAP_WIDTH),0,HUD_WIDTH,HUD_HEIGHT };
	float spriteSize = screenPosition.w / 3;
	cherrySpriteRect = Rect(screenPosition.x, MAP_HEIGHT / 3, spriteSize, spriteSize);
	strawberrySpriteRect = cherrySpriteRect;
	strawberrySpriteRect.y += spriteSize;
	orangeSpriteRect = strawberrySpriteRect;
	orangeSpriteRect.y += spriteSize;
	playerSpriteRect = Rect(screenPosition.x, MAP_HEIGHT - (spriteSize*1.5f), spriteSize, spriteSize);
	backgroundSpriteRect = screenPosition;

	InitTexts();
	InitTextRectPositions();
}

void HUD::SetSprites(Sprite* &_backgroundSprite, Sprite* &_cherrySprite, Sprite* &_strawberrySprite, Sprite* &_orangeSprite, Sprite* &_playerSprite) {
	backgroundSprite = _backgroundSprite;
	cherrySprite = _cherrySprite;
	strawberrySprite = _strawberrySprite;
	orangeSprite = _orangeSprite;
	playerSprite = _playerSprite;
}

void HUD::AddFruit(const FruitType &fruitType, const int &count) {
	int actualCount = fruitCount[(int)fruitType];
	int totalCount = actualCount + count;

	if (totalCount > MAX_PLAYER_FRUIT_COUNT) totalCount = MAX_PLAYER_FRUIT_COUNT;

	fruitCount[(int)fruitType] = totalCount;
}

void HUD::UpdateTextRects() {
	Renderer* rend = Renderer::Instance();

	if (score != std::stoi(scoreText.text)) {
		std::string scoreString = std::to_string(score);
		while (scoreString.size() != 4) {
			scoreString.insert(0, "0");
		}

		scoreText.text = scoreString;
		rend->LoadTextureText(scoreFont.id, scoreText);
	}

	Text* fruitText[] = { &cherryText ,&strawberryText , &orangeText };
	for (int i = 0; i < (int)FruitType::COUNT; i++) {
		int realCount = fruitCount[i];
		if (realCount != fruitText[i]->text.at(2) - '0') {
			fruitText[i]->text = std::string("X " + std::to_string(fruitCount[i]));
			rend->LoadTextureText(scoreFont.id, *fruitText[i]);
		}
	}
}

void HUD::InitTexts() {
	scoreText = {
		"scoreHUD",
		"0000",
		BLACK_COLOR
	};

	cherryText = {
		"cherryHUD",
		FRUIT_INIT_HUD_TEXT,
		BLACK_COLOR
	};

	strawberryText = {
		"strawberryHUD",
		FRUIT_INIT_HUD_TEXT,
		BLACK_COLOR
	};

	orangeText = {
		"orangeHUD",
		FRUIT_INIT_HUD_TEXT,
		BLACK_COLOR
	};

	Renderer* rend = Renderer::Instance();
	Text* textToLoad[] = { &scoreText , &cherryText ,&strawberryText , &orangeText };
	for (const auto &text : textToLoad) {
		rend->LoadTextureText(scoreFont.id, *text);
	}
}
void HUD::InitTextRectPositions() {
	scoreTextRect.SetPosition(Vector2(screenPosition.x + 10, MAP_HEIGHT / 10));

	float rectFruitTextPosX = cherrySpriteRect.x + (screenPosition.w / 3) + 10;
	cherryTextRect.SetPosition(Vector2(rectFruitTextPosX, cherrySpriteRect.y));
	strawberryTextRect.SetPosition(Vector2(rectFruitTextPosX, strawberrySpriteRect.y));
	orangeTextRect.SetPosition(Vector2(rectFruitTextPosX, orangeSpriteRect.y));
}

void HUD::UpdateTextRectSizes() {
	Renderer* rend = Renderer::Instance();
	scoreTextRect.SetSize(rend->GetTextureSize(scoreText.id));
	cherryTextRect.SetSize(rend->GetTextureSize(cherryText.id));
	strawberryTextRect.SetSize(rend->GetTextureSize(strawberryText.id));
	orangeTextRect.SetSize(rend->GetTextureSize(orangeText.id));
}

void HUD::UpdateText() {
	UpdateTextRects();
	UpdateTextRectSizes();
}

void HUD::Draw() {
	Renderer* rend = Renderer::Instance();

	// BACKGROUND
	rend->PushSprite(backgroundSprite->spriteId,
		backgroundSprite->texture,
		backgroundSpriteRect
	);

	// SCORE
	rend->PushImage(
		scoreText.id,
		scoreTextRect
	);

	// FRUITS SPRITES
	rend->PushSprite(cherrySprite->spriteId,
		cherrySprite->texture,
		cherrySpriteRect
	);

	rend->PushSprite(strawberrySprite->spriteId,
		strawberrySprite->texture,
		strawberrySpriteRect
	);

	rend->PushSprite(orangeSprite->spriteId,
		orangeSprite->texture,
		orangeSpriteRect
	);

	// FRUITS TEXT
	rend->PushImage(
		cherryText.id,
		cherryTextRect
	);

	rend->PushImage(
		strawberryText.id,
		strawberryTextRect
	);

	rend->PushImage(
		orangeText.id,
		orangeTextRect
	);

	// PLAYER LIVES
	Rect toPrint = playerSpriteRect;
	for (int i = 0; i < lives; i++) {
		rend->PushSprite(playerSprite->spriteId,
			playerSprite->texture,
			toPrint
		);
		toPrint.x += toPrint.w;
	}
}

void HUD::Update(const InputManager& input)
{
	score = mainPlayer->GetPoints();
	lives = mainPlayer->lives;
}


void HUD::HandleEvent(std::pair<Event, stream_buff::buffer> evt_info)
{
	switch (evt_info.first)
	{
	case Event::FRUIT_GOTTEN:
		AddFruit(stream_buff::get<FruitType>(evt_info.second, 1), 1);

		break;

	}
}

