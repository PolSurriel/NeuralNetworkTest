#include "Fruit.h"
#include "Consts.h"
#include "Time.h"
#include "Cherry.h"
#include "Strawberry.h"
#include "Orange.h"
#include "Types.h"

Fruit::Fruit()
{
	components.push_back(new SquareCollider(this, SQUARE_WIDTH_WORLD_SPACE, Collider::MASK_STATIC | Collider::MASK_TRIGGER));
}

void Fruit::OnCollision(Collider& _collider)
{
	switch (_collider.gameObject->tag)
	{
	case GameObject::Tag::PLAYABLE_CHARACTER:

		if(((PlayableCharacter*)(_collider.gameObject))->type == PlayableCharacter::Type::ESCAPIST)
		{
			stream_buff::buffer data = new stream_buff::data[2];

			stream_buff::insert<int>(data, &pointsValue, 0);
			stream_buff::insert<FruitType>(data, &type, 1);
			
			Throw(Event::FRUIT_GOTTEN, data);

			GameObject::Destroy(this);
		}
		
	}
}

FruitSpawnManager::FruitSpawnManager(Player* _player, Sprite &_cherrySprite, Sprite &_strawberrySprite, Sprite &_orangeSprite) : player(_player), cherrySprite(_cherrySprite), strawberrySprite(_strawberrySprite), orangeSprite(_orangeSprite)
{
	secondsToSpawnFruit = 0;
	ResetSpawnFruitCount(SECONDS_SPAWN_FRUIT);
}

void FruitSpawnManager::ResetSpawnFruitCount(const float &_secondsToSpawn) {
	secondsToSpawnFruit = _secondsToSpawn;
}

void FruitSpawnManager::Update(const InputManager& input)
{
	if (fruit == nullptr) {
		secondsToSpawnFruit -= FPSTime::deltaTimeInMilliseconds / 1000;
		if (secondsToSpawnFruit <= 0) {
			PlayableCharacter* randomPlayableCharacter = player->playableCharacters[rand() % PLAYABLE_CHARACTERS_FOR_EACH_PLAYER];
			if (Collision::SquareToSquareDetection(randomPlayableCharacter->position, SQUARE_RADIO_WORLD_SPACE, randomPlayableCharacter->initialPosition, SQUARE_RADIO_WORLD_SPACE)) {
				ResetSpawnFruitCount(SECONDS_RESPAWN_FRUIT_IF_COLLISION);
			}
			else {
				SpawnFruit(randomPlayableCharacter->initialPosition);
			}
		}
	}
}


void FruitSpawnManager::HandleEvent(std::pair<Event, stream_buff::buffer> evt_info)
{
	switch (evt_info.first)
	{
	case Event::FRUIT_GOTTEN:
		fruit = nullptr;
		ResetSpawnFruitCount(SECONDS_RESPAWN_FRUIT);
		break;
	}
}




void FruitSpawnManager::SpawnFruit(const Vector2& _position)
{
	FruitType fruitToSpawn;
	if (lastFruitSpawnedStreak == MAX_FRUIT_SPAWNED_STREAK) {
		do {
			fruitToSpawn = (FruitType)(rand() % (int)FruitType::COUNT);
		} while (fruitToSpawn != lastFruitSpawned);
	}
	else {
		fruitToSpawn = (FruitType)(rand() % (int)FruitType::COUNT);
	}

	if (fruitToSpawn == lastFruitSpawned) lastFruitSpawnedStreak++;
	else lastFruitSpawnedStreak = 0;

	Fruit* spawnFruit = nullptr;
	Sprite* spriteFruit = nullptr;
	switch (fruitToSpawn)
	{
	case FruitType::CHERRY:
		spawnFruit = new Cherry;
		spriteFruit = new Sprite(cherrySprite);
		break;
	case FruitType::STRAWBERRY:
		spawnFruit = new Strawberry;
		spriteFruit = new Sprite(strawberrySprite);
		break;
	case FruitType::ORANGE:
		spawnFruit = new Orange;
		spriteFruit = new Sprite(orangeSprite);
		break;
	}

	fruit = spawnFruit;
	fruit->actualSprite = spriteFruit;
	fruit->InitPosition(_position);

	GameObject::Instantiate(fruit, GameObject::DYNAMIC_DRAWABLE);
}



