#pragma once
#include "Entity.h"
#include "Consts.h"
#include "InputManager.h"


enum class BombermanPlayerState
{
	Dying,
	Died,
	Running,
	PoweredUp
};

class BombermanPlayer : public Entity
{
private:
	const float CHANGE_DIRECTION_COOLDOWN{ 0.15f };
	float changeDirectionCooldownCount;


	const float TIME_TO_RESPAWN{ (MILLISECONDS_DISINTEGRATING / 1000) + 1 };
	float timeToRespawnCount;


	const float EMPOWERED_COOLDOWN{ 10.0f };
	float empoweredCooldownCount;

public:

	enum class Type {
		PERSECUTOR, ESCAPIST
	};

	bool restoringPosition;

	double millisecondsDisintegratingCountdown;

	Type type;

	bool collidingWithEnemy;

	int upKey, downKey, leftKey, rightKey;

	bool movementInputActive;

	Vector2 desiredDirection;

	BombermanPlayerState state;

	void Empower();

	BombermanPlayer();
	void SetDirectionalKeys(const int& _upKey, const int& _downKey, const int& _leftKey, const int& _rightKey);
	void UpdateSprite();
	void HandleMapEdges();
	void HandleMovementInput(const InputManager& input);
	void Update(const InputManager& input) override;
	void OnCollision(Collider& _collider);

	void HandleEvent(std::pair<Event, stream_buff::buffer> evt_info) override;
};