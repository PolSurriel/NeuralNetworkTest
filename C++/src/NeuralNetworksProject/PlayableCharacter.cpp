#include "PlayableCharacter.h"
#include "Consts.h"
#include "Time.h"
#include "Fruit.h"
#include "Enemy.h"


PlayableCharacter::PlayableCharacter() {
	tag = Tag::PLAYABLE_CHARACTER;
	components.push_back(new SquareCollider(this, SQUARE_WIDTH_WORLD_SPACE));
	
	desiredDirection = VECTOR2_ZERO;
	millisecondsDisintegratingCountdown = 0;
	restoringPosition = false;
	state = PlayableCharacterState::Running;
}

void PlayableCharacter::Update(const InputManager& input)
{
	if (state == PlayableCharacterState::Dying) {

		timeToRespawnCount += FPSTime::deltaTimeInMilliseconds / 1000;

		if (timeToRespawnCount >= TIME_TO_RESPAWN) {
			state = PlayableCharacterState::Running;
			Throw(Event::RESET_POSITIONS);
		}
	}

	if(position.x > MAP_WIDTH)
	{
		position.x = -SQUARE_WIDTH_WORLD_SPACE;
	}else if(position.x < -SQUARE_WIDTH_WORLD_SPACE)
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
	

	if(state == PlayableCharacterState::PoweredUp)
	{
		empoweredCooldownCount += FPSTime::deltaTimeInMilliseconds /1000;
		if(empoweredCooldownCount >= EMPOWERED_COOLDOWN)
		{
			state = PlayableCharacterState::Running;
			Throw(Event::POWER_UP_TIME_OVER);
		}
	}


	
	if(input.keysDown[upKey]) desiredDirection = VECTOR2_UP;
	else if (input.keysDown[downKey]) desiredDirection = VECTOR2_DOWN;
	else if (input.keysDown[leftKey]) desiredDirection = VECTOR2_LEFT;
	else if (input.keysDown[rightKey]) desiredDirection = VECTOR2_RIGHT;

	if (movingDirection == VECTOR2_ZERO)
		movingDirection = desiredDirection;

	
	GetComponent<SquareCollider>()->width = SQUARE_WIDTH_WORLD_SPACE*0.99;

	rigidBody2D->velocity = movingDirection * speed;

	
	
	std::list<Collider*> hits = Collider::RayCastStatic(position + Vector2(SQUARE_RADIO_WORLD_SPACE, SQUARE_RADIO_WORLD_SPACE), desiredDirection, SQUARE_WIDTH_WORLD_SPACE, GetId());

	//Mehorar calculo del origen del hit 2
	std::list<Collider*> hits2 = Collider::RayCastStatic(movingDirection * SQUARE_RADIO_WORLD_SPACE*0.99f + position + Vector2(SQUARE_RADIO_WORLD_SPACE, SQUARE_RADIO_WORLD_SPACE), desiredDirection, SQUARE_WIDTH_WORLD_SPACE, GetId());

	if(changeDirectionCooldownCount > 0)
	{
		changeDirectionCooldownCount -= FPSTime::deltaTimeInMilliseconds / 1000;
	}
	
	if(desiredDirection != movingDirection && hits.empty() && hits2.empty())
	{
		if (desiredDirection == movingDirection * -1) {
			movingDirection = desiredDirection;
		}
		else if(changeDirectionCooldownCount <= 0) {

			Vector2 railPosition(
				trunc(position.x / SQUARE_WIDTH_WORLD_SPACE),
				trunc(position.y / SQUARE_HEIGHT_WORLD_SPACE)
			);

			railPosition = (railPosition)* SQUARE_WIDTH_WORLD_SPACE;

			if (movingDirection == VECTOR2_RIGHT) {
				railPosition.x += SQUARE_WIDTH_WORLD_SPACE;
				railPosition.x -= DISTANCE_BETWEEN_WALL_AND_PACMAN_RD;

			}

			else if (movingDirection == VECTOR2_DOWN) {
				railPosition.y += SQUARE_WIDTH_WORLD_SPACE;
				railPosition.y -= DISTANCE_BETWEEN_WALL_AND_PACMAN_RD;

			}
			
			else if (movingDirection == VECTOR2_UP) {
				railPosition.y += DISTANCE_BETWEEN_WALL_AND_PACMAN_UL;
			}

			else if (movingDirection == VECTOR2_LEFT) {
				railPosition.x += DISTANCE_BETWEEN_WALL_AND_PACMAN_UL;
			}

			float distanceToMove = speed * FPSTime::deltaTimeInMilliseconds;
			float distanceToRail = (railPosition - position + desiredDirection * -SQUARE_RADIO_WORLD_SPACE).GetMagnitude();

			
			if (distanceToMove <= distanceToRail) {
				movingDirection = desiredDirection;
				position = railPosition;
				changeDirectionCooldownCount = CHANGE_DIRECTION_COOLDOWN;

			}
		}
	}

	UpdateSprite();
};

void PlayableCharacter::UpdateSprite() {
	millisecondsInLife += FPSTime::deltaTimeInMilliseconds;

	int animationIndex;
	AnimatedSprite::Type spriteDirection;

	switch (state)
	{
	case PlayableCharacterState::Dying:
		if (millisecondsDisintegratingCountdown <= 0) {
			animationIndex = 0;
			spriteDirection = (AnimatedSprite::Type)-1;
		}
		else {
			millisecondsDisintegratingCountdown -= FPSTime::deltaTimeInMilliseconds;

			animationIndex = (static_cast<int>(MILLISECONDS_DISINTEGRATING - millisecondsDisintegratingCountdown) / MILLISECONDS_TO_UPDATE_SPRITE) % NUM_SPRITE_DISINTEGRATION_FRAMES;
			spriteDirection = AnimatedSprite::Type::DISINTEGRATION;
		}
		break;
	default:
		animationIndex = (static_cast<int>(millisecondsInLife) / MILLISECONDS_TO_UPDATE_SPRITE) % NUM_SPRITE_DIRECTION_FRAMES;

		if (movingDirection == VECTOR2_UP) {
			spriteDirection = AnimatedSprite::Type::UP;
		}
		else if (movingDirection == VECTOR2_DOWN) {
			spriteDirection = AnimatedSprite::Type::DOWN;
		}
		else if (movingDirection == VECTOR2_LEFT) {
			spriteDirection = AnimatedSprite::Type::LEFT;
		}
		else {
			spriteDirection = AnimatedSprite::Type::RIGHT;
		}
	}

	animatedSprite.SetTexture(spriteDirection, animationIndex);
}

void PlayableCharacter::SetDirectionalKeys(const int &_upKey, const int &_downKey, const int &_leftKey, const int &_rightKey) {
	upKey = { _upKey };
	downKey = { _downKey };
	leftKey = { _leftKey };
	rightKey = { _rightKey };
}

void PlayableCharacter::OnCollision(Collider &_collider) {
	return;
	switch (_collider.gameObject->tag)
	{
	case GameObject::Tag::WALL:
		position = Collision::SquareToSquareReaction(
			position, SQUARE_WIDTH_WORLD_SPACE/2, rigidBody2D->velocity,
			_collider.gameObject->position, SQUARE_WIDTH_WORLD_SPACE/2
		);
		break;
	case GameObject::Tag::PLAYABLE_CHARACTER:
		if (!restoringPosition && type == Type::ESCAPIST && type != ((PlayableCharacter*)(_collider.gameObject))->type) {
			//restoringPosition = true;
			if (!rigidBody2D->freezePosition) {
				GameObject::Throw(Event::MAINPLAYER_GETS_DAMAGE);
				rigidBody2D->freezePosition = true;
				state = PlayableCharacterState::Dying;
				timeToRespawnCount = 0;
				millisecondsDisintegratingCountdown = MILLISECONDS_DISINTEGRATING;
			}
		}
		break;
	case GameObject::Tag::POWER_UP:
		if(type == Type::ESCAPIST)
			GameObject::Throw(Event::POWER_UP_GOTTEN);

		break;

	case Tag::ENEMY:
		if (type == Type::ESCAPIST)
		{
			restoringPosition = true;
			if (state != PlayableCharacterState::PoweredUp)
			{
				if (!rigidBody2D->freezePosition) {
					GameObject::Throw(Event::MAINPLAYER_GETS_DAMAGE);
					rigidBody2D->freezePosition = true;
					state = PlayableCharacterState::Dying;
					timeToRespawnCount = 0;
					millisecondsDisintegratingCountdown = MILLISECONDS_DISINTEGRATING;

				}
			}
			else
			{
				Throw(Event::ENEMY_KILLED);
			}
		}
		else {
			position = rigidBody2D->lastPosition;
			_collider.gameObject->position = ((Entity*)_collider.gameObject)->rigidBody2D->lastPosition;

			_collider.gameObject->position = _collider.gameObject->position +((Entity*)_collider.gameObject)->rigidBody2D->velocity * speed * FPSTime::deltaTimeInMilliseconds / 1000;

			((Entity*)_collider.gameObject)->rigidBody2D->freezePosition = true;
			((Enemy*)_collider.gameObject)->freezedPos = true;
			((Enemy*)_collider.gameObject)->unfreezeTimeCount = 0;
			 
			std::cout << "Start" << std::endl;

			movingDirection = desiredDirection;

		}
		
	default:
		break;
	}
}


void PlayableCharacter::HandleEvent(std::pair<Event, stream_buff::buffer> evt_info)
{
	switch (evt_info.first)
	{
	case Event::MAINPLAYER_GETS_DAMAGE:
		rigidBody2D->freezePosition = true;
		restoringPosition = false;
		movingDirection = VECTOR2_ZERO;
		desiredDirection = VECTOR2_ZERO;
		break;

	case Event::RESET_POSITIONS:
		position = initialPosition;
		rigidBody2D->freezePosition = false;

		break;
	}
}


void PlayableCharacter::Empower()
{
	state = PlayableCharacterState::PoweredUp;
	empoweredCooldownCount = 0;

}


