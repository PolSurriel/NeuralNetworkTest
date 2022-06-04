#include "Entity.h"
#include "Consts.h"
#include "Game.h"

Entity::Entity() {
	speed = ENTITY_DEFAULT_SPEED;
	rigidBody2D = new RigidBody2D(this);
	components.push_back(rigidBody2D);
	delete actualSprite;
	actualSprite = &animatedSprite;
}

void Entity::InitPosition(Vector2 _position) {
	initialPosition = position = _position;
}

void Entity::SetSprite(AnimatedSprite* &sprite) {
	animatedSprite = *sprite;
}

void Entity::UpdateSprite() {
	millisecondsInLife += FPSTime::deltaTimeInMilliseconds;

	int animationIndex = ((int)millisecondsInLife / MILLISECONDS_TO_UPDATE_SPRITE) % NUM_SPRITE_DIRECTION_FRAMES;
	AnimatedSprite::Type spriteDirection;
	if (movingDirection == VECTOR2_UP) {
		spriteDirection = AnimatedSprite::Type::UP;
	}
	else if (movingDirection == VECTOR2_DOWN) {
		spriteDirection = AnimatedSprite::Type::DOWN;
	}
	else if (movingDirection == VECTOR2_LEFT) {
		spriteDirection = AnimatedSprite::Type::LEFT;
	}
	else if (movingDirection == VECTOR2_RIGHT) {
		spriteDirection = AnimatedSprite::Type::RIGHT;
	}

	animatedSprite.SetTexture(spriteDirection, animationIndex);
}

void Entity::ResetPosition() {
	position = initialPosition;
}

Vector2 Entity::GetVelocityVector() {
	return movingDirection * speed;
}

// not used
Vector2 Entity::GetNextPosition() {
	
	return VECTOR2_ZERO;
}
