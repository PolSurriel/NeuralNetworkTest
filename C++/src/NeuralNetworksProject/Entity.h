#pragma once
#include "GameObject.h"


/*
 * Las entidades son Objetos que se mueven.
 *
 * Por defecto tienen un puntero hacia su componente RigidBody2D
 * para facilitar el manejo de las fisicas.
 */

class Entity : public GameObject
{
protected:
	Vector2 movingDirection;
	AnimatedSprite animatedSprite;
	double millisecondsInLife;

	float speed;

public:
	Vector2 initialPosition;

	RigidBody2D* rigidBody2D;

	Entity();
	void InitPosition(Vector2 _position);
	void SetSprite(AnimatedSprite* &sprite);
	virtual void UpdateSprite();
	void ResetPosition();
	Vector2 GetNextPosition();
	Vector2 GetVelocityVector();
};
