#pragma once
#include "Entity.h"
#include "InputManager.h"


enum class EnemyState
{
	Running,
	EvadingPlayer,
	Died
};

class Enemy : public Entity
{
	Vector2 desiredDirection;

	const float changeDirectionProbability = 5;//%
	
	const float CHANGE_DIRECTION_COOLDOWN{ 0.25f };
	float changeDirectionCooldownCount;

	const float TO_UNFREEZE_POSITION{ 0.25f };
	
public:
	bool freezedPos;
	float unfreezeTimeCount;
	EnemyState state;
	Enemy();
	
	void Update(const InputManager& input) override;
	void UpdateDirection();
	void UpdateSprite();

	void OnCollision(Collider& _collider) override;

	void HandleEvent(std::pair<Event, stream_buff::buffer> evt_info) override;
	
};


/*
 *
 * Dado que el enemigo reaparece despues de ser eliminado,
 * no puede ser el quien se auto-instancie. Necesitamos un
 * manager que lleve la cuenta de los segundos.
 *
 * Esa es la funcion de este gameobject.
 * 
 */

class EnemySpawnManager : public GameObject
{

	Enemy * instantiated;

	AnimatedSprite* animatedSprite;
	
	Vector2 initialPosition;
	
	const float TIME_TO_SPAWN{ 2.0f };
	float timeToSpawnCount;
	
	enum class State
	{
		NORMAL,
		WAITING_TO_INSTANTIATE
	};

	EnemyState spawnState;
	State state;
	
public:

	EnemySpawnManager(const Vector2 & _initialPosition);

	~EnemySpawnManager();
	
	void SetSprite(AnimatedSprite* &sprite);
	void Update(const InputManager& input) override;
	void HandleEvent(std::pair<Event, stream_buff::buffer> evt_info) override;

	
};