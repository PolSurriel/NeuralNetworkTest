#pragma once
#include "Scene.h"
#include "Renderer.h"
#include "Consts.h"
#include "Collision.h"
#include "Time.h"
#include "AudioManager.h"

#include <list>

Scene::~Scene() {
	Renderer::Instance()->DeleteResources();
	AudioManager::Instance()->DeleteResources();
}

Splash::Splash() {
	state = SceneState::Running;
	secondsOnSplashScreen = 0;
	AudioManager* audioManager = AudioManager::Instance();
	audioManager->LoadAudio(PACMAN_INTRO_THEME_NAME, PACMAN_INTRO_THEME_PATH);
	audioManager->SetVolume(AudioManager::MAX_VOLUME);
	audioManager->PlayAudio(PACMAN_INTRO_THEME_NAME, 1);

	Renderer* rend = Renderer::Instance();
	titleFont = {
		"titleFont",
		PACMAN_FONT_PATH,
		SPLASH_TITLE_FONT_SIZE
	};

	subtitleFont = {
		"subtitleFont",
		GAMEPLAY_FONT_PATH,
		SPLASH_SUBTITLE_FONT_SIZE
	};

	rend->LoadFont(titleFont);
	rend->LoadFont(subtitleFont);

	Vector2 size;
	Vector2 pos;

	titleText = {
		"tag",
		"tag",
		ORANGE_COLOR
	};
	rend->LoadTextureText(titleFont.id, titleText);

	size = rend->GetTextureSize(titleText.id);
	pos.x = (SCREEN_WIDTH / 2.0f) - (size.x / 2);
	pos.y = (SCREEN_HEIGHT / 6.0f);

	titleRect.SetSize(size);
	titleRect.SetPosition(pos);

	subtitleText = {
		"wakaWaka",
		"\"waka...waka\"",
		BLACK_COLOR
	};
	rend->LoadTextureText(subtitleFont.id, subtitleText);

	size = rend->GetTextureSize(subtitleText.id);
	pos.x = (SCREEN_WIDTH / 2.0f) - (size.x / 2);
	pos.y = (SCREEN_HEIGHT / 2.0f);

	subtitleRect.SetSize(size);
	subtitleRect.SetPosition(pos);
}


void Splash::Update(const InputManager &input) {

	int inc = (secondsOnSplashScreen / ANIMATION_SECONDS_DURATION) * 255;
	titleText.color.r = (inc/2) + 255/2;
	titleText.color.g = 0;
	titleText.color.b = 0;
	subtitleText.color = {inc,inc,inc,0};
	Renderer::Instance()->LoadTextureText(titleFont.id, titleText);
	Renderer::Instance()->LoadTextureText(subtitleFont.id, subtitleText);

	switch (state)
	{
	case SceneState::Running:
		secondsOnSplashScreen += FPSTime::deltaTimeInMilliseconds / 1000;
		if (secondsOnSplashScreen >= ANIMATION_SECONDS_DURATION)
			state = SceneState::ToMenu;
		break;
	}
}

void Splash::Draw() {
	Renderer::Instance()->PushImage(
		titleText.id,
		titleRect
	);

	Renderer::Instance()->PushImage(
		subtitleText.id,
		subtitleRect
	);
}

Menu::Menu() {
	state = SceneState::Running;

	// INIT BUTTON
	Renderer* rend = Renderer::Instance();
	Rect buttonRect;
	Vector2 size;
	Vector2 pos;

	buttonFont = {
		"pacFont",
		PACMAN_FONT_PATH,
		MENU_FONT_SIZE
	};

	rend->LoadFont(buttonFont);

	Text normalPlayText{
		"normalPlayButtonMenu",
		"PLAY",
		RED_COLOR
	};
	
	Text hoverPlayText{
		"hoverPlayButtonMenu",
		"play",
		RED_COLOR
	};

	Button* playButton = new Button(buttonFont, normalPlayText, hoverPlayText);
	rend->LoadTextureText(buttonFont.id, hoverPlayText);

	rend->LoadTextureText(buttonFont.id, normalPlayText);
	// calculate buttonRect (size and pos)
	size = rend->GetTextureSize(normalPlayText.id);
	pos.x = (SCREEN_WIDTH / 2.0f) - (size.x / 2);
	pos.y = (size.y / 2);
	buttonRect.SetSize(size);
	buttonRect.SetPosition(pos);
	playButton->SetRect(buttonRect);

	options[0] = playButton;

	Text normalRankingText{
		"normalRankingButtonMenu",
		"RANKING",
		RED_COLOR
	};

	Text hoverRankingText{
		"hoverRankingButtonMenu",
		"ranking",
		RED_COLOR
	};

	Button* rankingButton = new Button(buttonFont, normalRankingText, hoverRankingText);
	rend->LoadTextureText(buttonFont.id, hoverRankingText);

	rend->LoadTextureText(buttonFont.id, normalRankingText);
	// calculate buttonRect (size and pos)
	size = rend->GetTextureSize(normalRankingText.id);
	pos.x = (SCREEN_WIDTH / 2.0f) - (size.x / 2);
	pos.y = (SCREEN_HEIGHT / 4.0f) + (size.y / 2);
	buttonRect.SetSize(size);
	buttonRect.SetPosition(pos);
	rankingButton->SetRect(buttonRect);

	options[1] = rankingButton;

	Text normalSoundText{
		"normalSoundButtonMenu",
		"SOUND OFF",
		RED_COLOR
	};

	Text hoverSoundText{
		"hoverSoundButtonMenu",
		"sound off",
		RED_COLOR
	};

	Text normalOffSoundText{
		"normalOffSoundButtonMenu",
		"SOUND ON",
		RED_COLOR
	};

	Text hoverOffSoundText{
		"hoverOffSoundButtonMenu",
		"sound on",
		RED_COLOR
	};

	SwitchButton* soundButton = new SwitchButton(buttonFont, normalSoundText, hoverSoundText, normalOffSoundText, hoverOffSoundText);
	rend->LoadTextureText(buttonFont.id, normalSoundText);
	rend->LoadTextureText(buttonFont.id, hoverSoundText);
	rend->LoadTextureText(buttonFont.id, normalOffSoundText);
	rend->LoadTextureText(buttonFont.id, hoverOffSoundText);
	// calculate buttonRect (size and pos)
	size = rend->GetTextureSize(normalSoundText.id);
	pos.x = (SCREEN_WIDTH / 2.0f) - (size.x / 2);
	pos.y = (2 * SCREEN_HEIGHT / 4.0f) + (size.y / 2);
	buttonRect.SetSize(size);
	buttonRect.SetPosition(pos);

	Rect auxiliarButtonRect;
	size = rend->GetTextureSize(normalOffSoundText.id);
	pos.x = (SCREEN_WIDTH / 2.0f) - (size.x / 2);
	auxiliarButtonRect.SetSize(size);
	auxiliarButtonRect.SetPosition(pos);
	soundButton->SetRect(buttonRect, auxiliarButtonRect);

	options[2] = soundButton;

	Text normalExitText{
		"normalExitButtonMenu",
		"EXIT",
		RED_COLOR
	};
	
	Text hoverExitText{
		"hoverExitButtonMenu",
		"exit",
		RED_COLOR
	};

	Button* exitButton = new Button(buttonFont, normalExitText, hoverExitText);
	rend->LoadTextureText(buttonFont.id, hoverExitText);

	rend->LoadTextureText(buttonFont.id, normalExitText);
	// calculate buttonRect (size and pos)
	size = rend->GetTextureSize(normalExitText.id);
	pos.x = (SCREEN_WIDTH / 2.0f) - (size.x / 2);
	pos.y = (3 * SCREEN_HEIGHT / 4.0f) + (size.y / 2);
	buttonRect.SetSize(size);
	buttonRect.SetPosition(pos);
	exitButton->SetRect(buttonRect);

	options[3] = exitButton;

	// INIT MUSIC
	AudioManager* audioManager = AudioManager::Instance();
	audioManager->LoadAudio(MENU_THEME_NAME, MENU_THEME_PATH);
	audioManager->SetVolume(AudioManager::MAX_VOLUME);
	audioManager->PlayAudio(MENU_THEME_NAME);

}

void Menu::Update(const InputManager &input) {

	int buttonPressed = -1;

	for (int i = 0; i < BUTTON_MENU_COUNT; i++) {
		options[i]->Update(input);
		if (options[i]->pressed)
			buttonPressed = i;
	}

	switch (buttonPressed)
	{
	case 0:
		state = SceneState::ToPlay;
		break;
	case 1:
		state = SceneState::ToRanking;
		break;
	case 2:
		switch (((SwitchButton*)options[2])->state)
		{
		case SwitchButton::State::ON:
			AudioManager::Instance()->ResumeAudio();
			break;
		case SwitchButton::State::OFF:
			AudioManager::Instance()->PauseAudio();
			break;
		}
		break;
	case 3:
		state = SceneState::Exit;
		break;
	default:
		break;
	}
}

void Menu::Draw() {
	for (auto &option : options)
		option->Draw();
}

Menu::~Menu() {
	for (auto &option : options)
		delete option;
}
