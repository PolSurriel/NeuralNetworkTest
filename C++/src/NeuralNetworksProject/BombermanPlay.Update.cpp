#include "BombermanPlayer.h"
#include "Consts.h"
#include "Time.h"
#include "Fruit.h"
#include "Enemy.h"


void BombermanPlayer::HandleMapEdges() {
	if (position.x > MAP_WIDTH)
	{
		position.x = -SQUARE_WIDTH_WORLD_SPACE;
	}
	else if (position.x < -SQUARE_WIDTH_WORLD_SPACE)
	{
		position.x = MAP_WIDTH;
	}

	if (position.y > MAP_HEIGHT)
	{
		position.y = -SQUARE_HEIGHT_WORLD_SPACE;
	}
	else if (position.y < -SQUARE_HEIGHT_WORLD_SPACE)
	{
		position.y = MAP_HEIGHT;
	}
}

void BombermanPlayer::HandleMovementInput(const InputManager& input) {

	if (input.keysPressed[upKey]) {
		desiredDirection = VECTOR2_UP;
		movementInputActive = true;
	}
	else if (input.keysPressed[downKey]) {
		desiredDirection = VECTOR2_DOWN;
		movementInputActive = true;
	}
	else if (input.keysPressed[leftKey]) {
		desiredDirection = VECTOR2_LEFT;
		movementInputActive = true;
	}
	else if (input.keysPressed[rightKey]) {
		desiredDirection = VECTOR2_RIGHT;
		movementInputActive = true;
	}
	else {
		movementInputActive = false;

	}

	
	// Do I want to change my movement direction?
	if (movementInputActive && movingDirection != desiredDirection) { 

		if (desiredDirection == movingDirection * -1) {
			movingDirection = desiredDirection;
		}
		else {

			Vector2 playerCenter = position + Vector2(SQUARE_RADIO_WORLD_SPACE, SQUARE_RADIO_WORLD_SPACE);

			auto hitsA = Collider::RayCastStatic(
				playerCenter + (movingDirection) *SQUARE_RADIO_WORLD_SPACE,
				desiredDirection,
				SQUARE_WIDTH_WORLD_SPACE,
				GetId()
			);


			auto hitsB = Collider::RayCastStatic(
				playerCenter + (movingDirection * -SQUARE_RADIO_WORLD_SPACE),
				desiredDirection,
				SQUARE_HEIGHT_WORLD_SPACE,
				GetId()
			);

			if (hitsA.size() != 0) {

				if (hitsB.size() == 0) {
					movingDirection = movingDirection * -1;
				}
				else {
					movingDirection = desiredDirection;
					return;
				}
			}


			Vector2 rayCastOriginOffset = Vector2(
				movingDirection.x > 0.0f ? 0.01f : SQUARE_WIDTH_WORLD_SPACE * 0.95f,
				movingDirection.y > 0.0f ? 0.01f : SQUARE_HEIGHT_WORLD_SPACE * 0.95f
			);
		
			//Is the path clear? let's check with a raycast;
			auto hits = Collider::RayCastStatic(
				position + rayCastOriginOffset,
				desiredDirection, 
				SQUARE_WIDTH_WORLD_SPACE, 
				GetId()
			);

			
			if (hits.size() == 0 || movingDirection == VECTOR2_ZERO) {
				movingDirection = desiredDirection;
			}
		}

		
	}
	
}


void BombermanPlayer::Update(const InputManager& input)
{

	if (state == BombermanPlayerState::Dying) {

		timeToRespawnCount += FPSTime::deltaTimeInMilliseconds / 1000;

		if (timeToRespawnCount >= TIME_TO_RESPAWN) {
			state = BombermanPlayerState::Running;
			Throw(Event::RESET_POSITIONS);
		}
	}

	HandleMapEdges();

	if (state == BombermanPlayerState::PoweredUp)
	{
		empoweredCooldownCount += FPSTime::deltaTimeInMilliseconds / 1000;
		if (empoweredCooldownCount >= EMPOWERED_COOLDOWN)
		{
			state = BombermanPlayerState::Running;
			Throw(Event::POWER_UP_TIME_OVER);
		}
	}


	HandleMovementInput(input);
	
	rigidBody2D->velocity = movingDirection * (movementInputActive?speed:0.0f);


	UpdateSprite();
}
