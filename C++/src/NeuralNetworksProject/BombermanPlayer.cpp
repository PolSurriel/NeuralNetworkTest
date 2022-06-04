#include "BombermanPlayer.h"
#include "Consts.h"
#include "Time.h"
#include "Fruit.h"
#include "Enemy.h"


BombermanPlayer::BombermanPlayer() {
	tag = Tag::PLAYABLE_CHARACTER;
	components.push_back(new CircleCollider(this, SQUARE_WIDTH_WORLD_SPACE));

	speed = 100.0f;
	movingDirection = VECTOR2_LEFT;
	millisecondsDisintegratingCountdown = 0;
	restoringPosition = false;
	state = BombermanPlayerState::Running;
	rigidBody2D->freezePosition = false;

	GetComponent<CircleCollider>()->radius = SQUARE_RADIO_WORLD_SPACE * 0.9f;

}



void BombermanPlayer::UpdateSprite() {
	millisecondsInLife += FPSTime::deltaTimeInMilliseconds;

	int animationIndex;
	AnimatedSprite::Type spriteDirection;

	switch (state)
	{
	case BombermanPlayerState::Dying:
		if (millisecondsDisintegratingCountdown <= 0) {
			animationIndex = 0;
			spriteDirection = (AnimatedSprite::Type) - 1;
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

void BombermanPlayer::SetDirectionalKeys(const int& _upKey, const int& _downKey, const int& _leftKey, const int& _rightKey) {
	upKey = { _upKey };
	downKey = { _downKey };
	leftKey = { _leftKey };
	rightKey = { _rightKey };
}

void BombermanPlayer::OnCollision(Collider& _collider) {
	
	switch (_collider.gameObject->tag)
	{
	case GameObject::Tag::WALL:
		
		position = Collision::CircleToSquareReaction(
			position, 
			SQUARE_RADIO_WORLD_SPACE, 
			rigidBody2D->lastPosition,
			_collider.gameObject->position, 
			SQUARE_WIDTH_WORLD_SPACE
		);


		break;
	case GameObject::Tag::PLAYABLE_CHARACTER:
		if (!restoringPosition && type == Type::ESCAPIST && type != ((BombermanPlayer*)(_collider.gameObject))->type) {
			//restoringPosition = true;
			if (!rigidBody2D->freezePosition) {
				GameObject::Throw(Event::MAINPLAYER_GETS_DAMAGE);
				rigidBody2D->freezePosition = true;
				state = BombermanPlayerState::Dying;
				timeToRespawnCount = 0;
				millisecondsDisintegratingCountdown = MILLISECONDS_DISINTEGRATING;
			}
		}
		break;
	case GameObject::Tag::POWER_UP:
		if (type == Type::ESCAPIST)
			GameObject::Throw(Event::POWER_UP_GOTTEN);

		break;

	case Tag::ENEMY:
		if (type == Type::ESCAPIST)
		{
			restoringPosition = true;
			if (state != BombermanPlayerState::PoweredUp)
			{
				if (!rigidBody2D->freezePosition) {
					
					GameObject::Throw(Event::MAINPLAYER_GETS_DAMAGE);
					rigidBody2D->freezePosition = true;
					state = BombermanPlayerState::Dying;
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

			_collider.gameObject->position = _collider.gameObject->position + ((Entity*)_collider.gameObject)->rigidBody2D->velocity * speed * FPSTime::deltaTimeInMilliseconds / 1000;

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


void BombermanPlayer::HandleEvent(std::pair<Event, stream_buff::buffer> evt_info)
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


void BombermanPlayer::Empower()
{
	state = BombermanPlayerState::PoweredUp;
	empoweredCooldownCount = 0;

}


