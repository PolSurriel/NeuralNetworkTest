#include "Component.h"
#include "Time.h"
#include "Consts.h"


std::set<RigidBody2D*> RigidBody2D::rigidBody2D;
std::set<Collider*> Collider::colliders;
std::set<Collider*> Collider::staticColliders;

#pragma region  Collider
Collider::Collider(GameObject * _owner, short mask) : Component(_owner)
{

	isTrigger = mask & MASK_TRIGGER;
	isStatic = mask & MASK_STATIC;
	
	if (isStatic)
		staticColliders.insert(this);
	else
		colliders.insert(this);
	
}

Collider::~Collider()
{
	if (isStatic)
		staticColliders.erase(this);
	else
		colliders.erase(this);
	
}


void Collider::HandleCollisions()
{
	for (auto &dynamicCollider : Collider::colliders) {
		for (auto &otherDynamicCollider : dynamicCollider->colliders) {
			if (dynamicCollider->gameObject->GetId() != otherDynamicCollider->gameObject->GetId()) {
				if (dynamicCollider->Collides(otherDynamicCollider)) {
					dynamicCollider->gameObject->OnCollision(*otherDynamicCollider);
				}
			}
		}

		for (auto &staticCollider : dynamicCollider->staticColliders) {
			if (dynamicCollider->gameObject->GetId() != staticCollider->gameObject->GetId()) {
				if (dynamicCollider->Collides(staticCollider)) {
					dynamicCollider->gameObject->OnCollision(*staticCollider);
					staticCollider->gameObject->OnCollision(*dynamicCollider);
				}
			}
		}
	}
}


Vector2 Collider::GetPosition() const
{
	return localPosition + gameObject->position;
}

std::list<Collider*> Collider::RayCastAll(Vector2 origin, Vector2 direction, float distance, int ignoreId)
{
	std::list<Collider*> list;

	Collider::RayCast(list, Collider::colliders, origin, direction, distance, ignoreId);
	Collider::RayCast(list, Collider::staticColliders, origin, direction, distance, ignoreId);

	return list;
}

std::list<Collider*> Collider::RayCastDynamic(Vector2 origin, Vector2 direction, float distance, int ignoreId)
{
	std::list<Collider*> list;

	Collider::RayCast(list, Collider::colliders, origin, direction, distance, ignoreId);

	return list;
}

std::list<Collider*> Collider::RayCastStatic(Vector2 origin, Vector2 direction, float distance, int ignoreId)
{
	std::list<Collider*> list;

	Collider::RayCast(list, Collider::staticColliders, origin, direction, distance, ignoreId);

	return list;
}

void Collider::RayCast(std::list<Collider*> & list, const std::set<Collider*>& collection, Vector2 origin, Vector2 direction, float distance, int ignoreId)
{
	for (auto & collider : collection)
	{
		if (collider->gameObject->GetId() == ignoreId)
			continue;

		if (collider->Collides(origin, direction, distance)) {
			list.push_back(collider);
		}
	}

}


const std::type_info& Collider::GetType()
{
	return typeid(Collider);
}



#pragma  endregion 


#pragma region CircleCollider

const std::type_info& CircleCollider::GetType()
{
	return typeid(CircleCollider);
}

CircleCollider::CircleCollider(GameObject* _owner, float _radio, short mask) : Collider(_owner, mask), radius(_radio)
{

}

bool CircleCollider::Collides(Vector2 origin, Vector2 direction, float distance) const
{
	return false;
}

bool CircleCollider::Collides(const Collider* collider) const
{
	return collider->Collides(this);
}

bool CircleCollider::Collides(const SquareCollider* collider) const
{
	auto circlePos = GetPosition() + Vector2(radius, radius);
	auto squarePos = collider->GetPosition();
	
	auto squareCenter = squarePos + Vector2(collider->width, collider->width)*0.5f;

	auto toCenter = squareCenter - circlePos;

	if (toCenter.GetMagnitude() < radius)
		return true;

	toCenter.Normalize();
	
	circlePos = circlePos + toCenter * radius;

	return
		circlePos.x > squarePos.x &&
		circlePos.x < squarePos.x + collider->width &&
		circlePos.y > squarePos.y &&
		circlePos.y < squarePos.y + collider->width;

}

bool CircleCollider::Collides(const CircleCollider* collider) const
{
	float dist = (GetPosition() - collider->GetPosition()).GetMagnitude();
	return dist < radius + collider->radius;
}



#pragma endregion

#pragma region SquareCollider

bool SquareCollider::Collides(const Collider* collider) const
{
	return collider->Collides(this);
}

SquareCollider::SquareCollider(GameObject* _owner, float _width, short mask) : Collider(_owner, mask), width(_width) {}

bool SquareCollider::Collides(const SquareCollider* collider) const
{
	return Collision::SquareToSquareDetection(
		this->GetPosition(),
		this->width/2,
		collider->GetPosition(),
		collider->width/2
	);
}

bool SquareCollider::Collides(const CircleCollider* collider) const
{
	return collider->Collides(this);
}

bool SquareCollider::Collides(Vector2 origin, Vector2 direction, float distance) const
{
	return Collision::LineToRect(
		origin,
		origin + (direction * distance),
		GetPosition(),
		width,
		width
	);
}

const std::type_info& SquareCollider::GetType()
{
	return typeid(SquareCollider);
}


#pragma endregion 

#pragma region RigidBody2D
RigidBody2D::RigidBody2D(GameObject * _owner) : Component(_owner)
{
	rigidBody2D.insert(this);
	velocity = VECTOR2_ZERO;
}

RigidBody2D::~RigidBody2D()
{
	rigidBody2D.erase(this);
}

void RigidBody2D::UpdatePhysics()
{
	lastPosition = gameObject->position;

	if(!freezePosition)
		gameObject->position = gameObject->position + velocity * (FPSTime::deltaTimeInMilliseconds / 1000);
}


void RigidBody2D::ApplyPhysics()
{
	for (auto& rigidBody : RigidBody2D::rigidBody2D) {
		rigidBody->UpdatePhysics();
	}
}



const std::type_info& RigidBody2D::GetType()
{
	return typeid(RigidBody2D);
}



#pragma endregion 





Component::~Component()
{
	
}

