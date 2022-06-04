#include "Canvas.h"
#include "Renderer.h"
#include "Consts.h"
#include "AudioManager.h"

void Canvas::Init() {
	InitFont();
	InitText();
	InitRects();
}

void Canvas::InitFont() {
	textFont = {
		"canvasFont",
		PACMAN_FONT_PATH,
		START_GAME_FONT_SIZE
	};

	Renderer::Instance()->LoadFont(textFont);
}

void Canvas::InitText() {
	runText[0] = {
			"startGameCanvas1",
			"PrEsS sPaCe",
			RED_COLOR
	};

	runText[1] = {
			"startGameCanvas2",
			"To StArT........",
			RED_COLOR
	};

	Renderer* rend = Renderer::Instance();
	for (const auto &textToLoad : runText) {
		rend->LoadTextureText(textFont.id, textToLoad);
	}
}

void Canvas::InitRects() {
	canvasRect = Rect(VECTOR2_ZERO, Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));

	Renderer* rend = Renderer::Instance();
	for (int i = 0; i < RUN_TEXT_CANVAS_LINES; i++) {
		Vector2 textSize = rend->GetTextureSize(runText[i].id);
		runTextRect[i].SetSize(textSize);

		runTextRect[i].SetPosition(Vector2(
			(MAP_WIDTH / 2.0f) - (textSize.x / 2),
			((MAP_HEIGHT / 2.0f) - ((textSize.y / 2) * ((i == 0)? 1.5f : -1.5f)))
		));

		rend->LoadTextureText(textFont.id, runText[i]);
	}
}


void Canvas::Draw() {
	Renderer* rend = Renderer::Instance();

	// BACKGROUND
	rend->PushSprite(backgroundSprite->spriteId,
		backgroundSprite->texture,
		canvasRect
	);

	// TEXT TO RUN GAME
	for (int i = 0; i < RUN_TEXT_CANVAS_LINES; i++) {
		rend->PushImage(
			runText[i].id,
			runTextRect[i]
		);
	}
}

ButtonCanvas::ButtonCanvas() {}

void ButtonCanvas::Init() {
	InitFont();
	InitText();
	InitRects();
	InitButton();
}

void ButtonCanvas::InitFont() {
	Canvas::InitFont();
	stopFont = {
		"canvasStopFont",
		PACMAN_FONT_PATH,
		STOP_GAME_FONT_SIZE
	};

	Renderer::Instance()->LoadFont(stopFont);
}

void ButtonCanvas::InitText() {
	stopText = {
			"stopText",
			"StOp",
			RED_COLOR
	};

	runText[0] = {
			"releaseGameCanvas1",
			"PrEsS sPaCe",
			RED_COLOR
	};

	runText[1] = {
			"releaseGameCanvas2",
			"To ReLeAsE",
			RED_COLOR
	};

	Renderer* rend = Renderer::Instance();
	rend->LoadTextureText(stopFont.id, stopText);
	for (const auto &textToLoad : runText) {
		rend->LoadTextureText(textFont.id, textToLoad);
	}
}

void ButtonCanvas::InitButton() {
	Text normalOnSoundText{
		"normalOnSoundButtonCanvas",
		"SOUND OFF",
		WHITE_COLOR
	};

	Text hoverOnSoundText{
		"hoverOnSoundButtonCanvas",
		"sound off",
		WHITE_COLOR
	};

	Text normalOffSoundText{
		"normalOffSoundButtonCanvas",
		"SOUND ON",
		WHITE_COLOR
	};

	Text hoverOffSoundText{
		"hoverOffSoundButtonCanvas",
		"sound on",
		WHITE_COLOR
	};

	Renderer* rend = Renderer::Instance();
	rend->LoadTextureText(textFont.id, normalOnSoundText);
	rend->LoadTextureText(textFont.id, hoverOnSoundText);
	rend->LoadTextureText(textFont.id, normalOffSoundText);
	rend->LoadTextureText(textFont.id, hoverOffSoundText);

	soundButton = SwitchButton(textFont, normalOnSoundText, hoverOnSoundText, normalOffSoundText, hoverOffSoundText);
	
	Vector2 size;
	Vector2 pos;
	Rect onButtonRect;
	Rect offButtonRect;

	float initialXButtonPos = MAP_WIDTH / 2.0f;

	size = rend->GetTextureSize(normalOnSoundText.id);
	pos = Vector2(
		initialXButtonPos - (size.x / 2),
		(MAP_HEIGHT / 2.0f) + 100
	);

	onButtonRect.SetSize(size);
	onButtonRect.SetPosition(pos);

	size = rend->GetTextureSize(normalOffSoundText.id);
	pos.x = initialXButtonPos - (size.x / 2);

	offButtonRect.SetSize(size);
	offButtonRect.SetPosition(pos);

	soundButton.SetRect(onButtonRect, offButtonRect);
}

void ButtonCanvas::InitRects() {
	canvasRect = Rect(VECTOR2_ZERO, Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));

	Renderer* rend = Renderer::Instance();
	Vector2 stopTextSize = rend->GetTextureSize(stopText.id);
	stopTextRect.SetSize(stopTextSize);
	float initialXText = MAP_WIDTH / 2.0f;
	Vector2 stopTextPosition = Vector2(
		initialXText - (stopTextSize.x / 2),
		(MAP_HEIGHT / 6.0f) - (stopTextSize.y / 2)
	);
	stopTextRect.SetPosition(stopTextPosition);
	rend->LoadTextureText(stopFont.id, stopText);

	float initialYRunText = stopTextPosition.y + stopTextSize.y + 30;

	for (int i = 0; i < RUN_TEXT_CANVAS_LINES; i++) {
		Vector2 textSize = rend->GetTextureSize(runText[i].id);
		runTextRect[i].SetSize(textSize);

		runTextRect[i].SetPosition(Vector2(
			initialXText - (textSize.x / 2),
			initialYRunText + (textSize.y + 20) * i + 20
		));

		rend->LoadTextureText(textFont.id, runText[i]);
	}
}

void ButtonCanvas::Update(const InputManager &input) {
	soundButton.Update(input);
	
	if (soundButton.pressed) {
		switch (soundButton.state)
		{
		case SwitchButton::State::ON:
			AudioManager::Instance()->ResumeAudio();
			break;
		case SwitchButton::State::OFF:
			AudioManager::Instance()->PauseAudio();
			break;
		}
	}
}

void ButtonCanvas::Draw() {
	Renderer* rend = Renderer::Instance();

	// BACKGROUND
	rend->PushSprite(backgroundSprite->spriteId,
		backgroundSprite->texture,
		canvasRect
	);

	// STOP TEXT
	rend->PushImage(
		stopText.id,
		stopTextRect
	);

	// SOUND BUTTON
	soundButton.Draw();

	// TEXT TO RUN GAME
	for (int i = 0; i < RUN_TEXT_CANVAS_LINES; i++) {
		rend->PushImage(
			runText[i].id,
			runTextRect[i]
		);
	}
}