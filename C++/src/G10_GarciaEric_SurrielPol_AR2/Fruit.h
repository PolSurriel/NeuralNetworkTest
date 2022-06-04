#pragma once
#include "GameObject.h"
#include "Player.h"

enum class FruitType {
	CHERRY, STRAWBERRY, ORANGE, COUNT
};

class Fruit : public GameObject
{
public:
	FruitType type;
	int pointsValue;

	void OnCollision(Collider& _collider) override;
	
	Fruit();
};


/*
 *
 *Al igual que el enemigo, cuando la fruta no existe
 *necesita que alguien/algo lleve la cuenta por el.
 *
 *Este objeto invisible lo unico que hace es gestionar
 *el respawn de las frutas.
 *
 * 
 */

class FruitSpawnManager : public GameObject
{
public:
	float secondsToSpawnFruit;
	Fruit* fruit;
	Player* player;

	FruitType lastFruitSpawned;
	int lastFruitSpawnedStreak;

	Sprite cherrySprite;
	Sprite strawberrySprite;
	Sprite orangeSprite;

	FruitSpawnManager(Player* _player, Sprite &_cherrySprite, Sprite &_strawberrySprite, Sprite &_orangeSprite);
	void Update(const InputManager& input) override;

	void ResetSpawnFruitCount(const float &_secondsToSpawn);

	void SpawnFruit(const Vector2 &_position);

	void HandleEvent(std::pair<Event, stream_buff::buffer> evt_info) override;
	
};
