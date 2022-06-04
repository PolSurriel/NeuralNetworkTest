#include "Scene.h"
#include "Collision.h"
#include "Consts.h"
#include "Renderer.h"
#include "Time.h"
#include "AudioManager.h"

#include "Cherry.h"
#include "Strawberry.h"
#include "Orange.h"
#include "GameData.h"


TagPlay::TagPlay(std::list<GameObject*> _walls, std::list<PowerUp*> _powerUps, Escapist* _firstPlayer, Player* _secondPlayer, EnemySpawnManager* _enemySpawnManager)
	: walls(_walls), powerUps(_powerUps), firstPlayer(_firstPlayer), secondPlayer(_secondPlayer), enemySpawnManager(_enemySpawnManager)
{
	startCanvas = new Canvas;
	pauseCanvas = new ButtonCanvas;
	startCanvas->Init();
	pauseCanvas->Init();
	activeCanvas.insert(startCanvas);

	Renderer::Instance()->LoadTexture(PACMAN_SPRITE_SHEET_ID, PACMAN_SPRITE_SHEET_PATH);
	Renderer::Instance()->LoadTexture(ENEMY_SPRITE_SHEET_ID, ENEMY_SPRITE_SHEET_PATH);

	AudioManager::Instance()->LoadAudio(GAME_THEME_NAME, GAME_THEME_PATH);
	Init();

	state = SceneState::StartGame;

};


void TagPlay::Init() {

	hud.mainPlayer = firstPlayer;
	
	// calculate sprite positions at spriteSheet
	Vector2 pacmanSpriteSheetImageSize = Renderer::Instance()->GetTextureSize(PACMAN_SPRITE_SHEET_ID);
	float pacmanSpriteWidth = pacmanSpriteSheetImageSize.x / 8;
	float pacmanSpriteHeight = pacmanSpriteSheetImageSize.y / 8;

	Vector2 enemySpriteSheetImageSize = Renderer::Instance()->GetTextureSize(ENEMY_SPRITE_SHEET_ID);
	float enemySpriteWidth = enemySpriteSheetImageSize.x / 4;
	float enemySpriteHeight = enemySpriteSheetImageSize.y;

	Vector2 pacmanSpriteSize = { pacmanSpriteWidth , pacmanSpriteHeight };
	Vector2 enemySpriteSize = { enemySpriteWidth , enemySpriteHeight };

	Vector2 canvasSpritePosition = { 0 , pacmanSpriteHeight * 7 };

	Vector2 firstPlayerSpritePosition = { pacmanSpriteWidth * 0 , 0 };
	Vector2 secondPlayerSpritePosition = { 0, pacmanSpriteHeight * 2 };
	Vector2 enemySpritePosition = { 0 , pacmanSpriteHeight };

	Vector2 wallSpritePosition = { pacmanSpriteWidth * 4 , pacmanSpriteHeight * 6 };
	Vector2 powerUpSpritePosition = { pacmanSpriteWidth * 6 , pacmanSpriteHeight * 6 };
	Vector2 hudSpritePosition = { pacmanSpriteWidth * 7 , pacmanSpriteHeight * 6 };

	Vector2 cherrySpritePosition = { 0 , pacmanSpriteHeight * 6 };
	Vector2 strawberrySpritePosition = { pacmanSpriteWidth , pacmanSpriteHeight * 6 };
	Vector2 orangeSpritePosition = { pacmanSpriteWidth * 2 , pacmanSpriteHeight * 6 };

	Rect firstPlayerRect(firstPlayerSpritePosition, pacmanSpriteSize);
	Rect secondPlayerRect(secondPlayerSpritePosition, pacmanSpriteSize);
	Rect enemyPacmanSpriteSheetRect(enemySpritePosition, pacmanSpriteSize);
	Rect enemyEnemySpriteSheetRect(VECTOR2_ZERO, enemySpriteSize);

	AnimatedSprite* firstPlayerAnimatedSprite = new AnimatedSprite(PACMAN_SPRITE_SHEET_ID, firstPlayerRect);
	AnimatedSprite* secondPlayerAnimatedSprite = new AnimatedSprite(PACMAN_SPRITE_SHEET_ID, secondPlayerRect);
	AnimatedSprite* enemyAnimatedSprite = new AnimatedSprite(PACMAN_SPRITE_SHEET_ID, enemyPacmanSpriteSheetRect);

	// init sprites
	firstPlayerSprite = firstPlayerAnimatedSprite;
	secondPlayerSprite = secondPlayerAnimatedSprite;
	enemySprite = enemyAnimatedSprite;

	// direction Animations
	int i = 1;
	for (; i < 8; i++) {
		firstPlayerRect.x += pacmanSpriteSize.x;
		firstPlayerAnimatedSprite->PushTexture(i, PACMAN_SPRITE_SHEET_ID, firstPlayerRect);

		secondPlayerRect.x += pacmanSpriteSize.x;
		secondPlayerAnimatedSprite->PushTexture(i, PACMAN_SPRITE_SHEET_ID, secondPlayerRect);

		enemyPacmanSpriteSheetRect.x += pacmanSpriteSize.x;
		enemyAnimatedSprite->PushTexture(i, PACMAN_SPRITE_SHEET_ID, enemyPacmanSpriteSheetRect);
	}

	// escapist desintegration part1
	firstPlayerRect.x = pacmanSpriteWidth * 4;
	firstPlayerRect.y = pacmanSpriteHeight * 4;
	for (; i < 12; i++) {
		firstPlayerAnimatedSprite->PushTexture(i, PACMAN_SPRITE_SHEET_ID, firstPlayerRect);
		firstPlayerRect.x += pacmanSpriteSize.x;
	}

	// escapist desintegration part2
	firstPlayerRect.x = 0;
	firstPlayerRect.y += pacmanSpriteSize.y;
	for (; i < 20; i++) {
		firstPlayerAnimatedSprite->PushTexture(i, PACMAN_SPRITE_SHEET_ID, firstPlayerRect);
		firstPlayerRect.x += pacmanSpriteSize.x;
	}

	// enemy power up
	for (int i = 8; i < 12; i++) {
		enemyAnimatedSprite->PushTexture(i, ENEMY_SPRITE_SHEET_ID, enemyEnemySpriteSheetRect);
		enemyEnemySpriteSheetRect.x += enemySpriteSize.x;
	}

	wallSprite = new Sprite(PACMAN_SPRITE_SHEET_ID, Rect(wallSpritePosition, pacmanSpriteSize));
	powerUpSprite = new Sprite(PACMAN_SPRITE_SHEET_ID, Rect(powerUpSpritePosition, pacmanSpriteSize));

	cherrySprite = new Sprite(PACMAN_SPRITE_SHEET_ID, Rect(cherrySpritePosition, pacmanSpriteSize));
	strawberrySprite = new Sprite(PACMAN_SPRITE_SHEET_ID, Rect(strawberrySpritePosition, pacmanSpriteSize));
	orangeSprite = new Sprite(PACMAN_SPRITE_SHEET_ID, Rect(orangeSpritePosition, pacmanSpriteSize));

	hudSprite = new Sprite(PACMAN_SPRITE_SHEET_ID, Rect(hudSpritePosition, pacmanSpriteSize));
	canvasSprite = new Sprite(PACMAN_SPRITE_SHEET_ID, Rect(canvasSpritePosition, pacmanSpriteSize));

	// assign sprites
	for (auto* &firstPlayableCharacter : firstPlayer->playableCharacters) {
		firstPlayableCharacter->SetSprite(firstPlayerAnimatedSprite);
	}

	for (auto* &secondPlayableCharacter : secondPlayer->playableCharacters) {
		secondPlayableCharacter->SetSprite(secondPlayerAnimatedSprite);
	}

	for (auto* &wall : walls) {
		wall->SetSprite(wallSprite);
	}

	for (auto* &powerUp : powerUps) {
		powerUp->SetSprite(powerUpSprite);
	}

	enemySpawnManager->SetSprite(enemyAnimatedSprite);

	hud.SetSprites(hudSprite, cherrySprite, strawberrySprite, orangeSprite, firstPlayerSprite);

	startCanvas->backgroundSprite = canvasSprite;
	pauseCanvas->backgroundSprite = canvasSprite;

	GameObject::Instantiate(firstPlayer->playableCharacters[0], GameObject::DYNAMIC_DRAWABLE);
	GameObject::Instantiate(firstPlayer->playableCharacters[1], GameObject::DYNAMIC_DRAWABLE);
	GameObject::Instantiate(secondPlayer->playableCharacters[0], GameObject::DYNAMIC_DRAWABLE);
	GameObject::Instantiate(secondPlayer->playableCharacters[1], GameObject::DYNAMIC_DRAWABLE);
	GameObject::Instantiate(enemySpawnManager, GameObject::DYNAMIC);
	GameObject::Instantiate(new FruitSpawnManager(firstPlayer, *cherrySprite, *strawberrySprite, *orangeSprite), GameObject::DYNAMIC);
	GameObject::Instantiate(firstPlayer, GameObject::DYNAMIC);
	GameObject::Instantiate(&hud, GameObject::DYNAMIC);

	for (auto & e : powerUps)
		GameObject::Instantiate(e, GameObject::DYNAMIC_DRAWABLE);

	for (auto* &wall : walls)
		GameObject::Instantiate(wall, GameObject::DRAWABLE);

	GameObject::ManageInstances();
	
}

void TagPlay::OnExit()
{

	for (auto & e : walls) {
		delete e;
	}

	delete firstPlayer;
	delete secondPlayer;
	delete startCanvas;
	delete pauseCanvas;

	delete enemySpawnManager;

	GameObject::drawableObjects = std::set<GameObject*>();
	GameObject::dynamicGameObjects = std::set<GameObject*>();


}

void TagPlay::Update(const InputManager &input)
{
	
	switch (state)
	{
	case SceneState::Running:
		RunningUpdate(input);
		if (input.keysDown[(int)InputManager::Keys::P]) {
			state = SceneState::Paused;
			activeCanvas.insert(pauseCanvas);
		}else if(firstPlayer->lives <= 0)
		{
			state = SceneState::GameOver;
		}
		break;
	case SceneState::StartGame:
		if (input.keysDown[(int)InputManager::Keys::SPACE]) {
			state = SceneState::Running;
			activeCanvas.erase(startCanvas);
			AudioManager::Instance()->SetVolume(AudioManager::MAX_VOLUME);
			AudioManager::Instance()->PlayAudio(GAME_THEME_NAME);
		} else if (input.keysDown[(int)InputManager::Keys::ESC]) {
			state = SceneState::ToMenu;
		}
		break;
	case SceneState::Paused:
		if (input.keysDown[(int)InputManager::Keys::SPACE]) {
			state = SceneState::Running;
			activeCanvas.erase(pauseCanvas);
		}
		else if (input.keysDown[(int)InputManager::Keys::ESC]) {
			state = SceneState::ToMenu;

		}
		else {
			pauseCanvas->Update(input);
		}
		break;
	case SceneState::Exit:
		break;
	case SceneState::ToPlay:
		break;
	case SceneState::ToRanking:
		break;
	case SceneState::GameOver:{

			std::cout << "Nombre de usuario: " << std::endl;

			std::string name;
			std::getline(std::cin, name);
			
			GameData::SaveRanking({firstPlayer->GetPoints(), name });

			std::vector<MatchInfo> info = GameData::GetRanking();

			for(auto & e : info)
			{
				std::cout << e.name << "  W: " << e.points << std::endl;
			}
			
			state = SceneState::ToRanking;
			
		}
		break;
	case SceneState::ToMenu:
		break;
	default:
		break;
	}

	hud.UpdateText();
	
}