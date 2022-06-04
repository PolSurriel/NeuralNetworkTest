#pragma once

#include <set>
#include <list>
#include "Types.h"
#include "Collision.h"
#include "GameObject.h"



/*
 * Leer comentario de la clase GameObject para comprender mejor
 * como hemos llegado a la conclusion de que debiamos utilizar
 * el visitor pattern con agregacion bidireccional.
 *
 *
 *Los Componentes no tienen sentido si no existen dentro de un GameObject.
 *
 *
 */

class GameObject;
class SquareCollider;
class CircleCollider;


class Component
{
public:
	GameObject *gameObject;

	/*
	 *No podemos obtener el tipo de un componente con el typeid()
	 *de un puntero a component. Por ello hemos tenido que crear
	 *este metodo virtual puro para que los hijos lo sobreescriban
	 *devolviendo el typeid de su clase;
	 * 
	 */
	virtual const std::type_info& GetType() = 0;
	
	Component(GameObject * _owner){
		gameObject = _owner;
	}

	virtual ~Component();
	
};

class Collider : public Component
{
	
public:
	const std::type_info& GetType() override;

	//Los triggers no provocan reacciones en las fisicas.
	static const short MASK_TRIGGER = 1; // 0001
	//Los colliders estaticos no se mueven, por tanto no
	//hace falta comprobar sus colisiones entre si.
	static const short MASK_STATIC = 2;  // 0010
	
	bool isStatic;
	bool isTrigger;

	~Collider();
	
	Vector2 localPosition;
	Vector2 GetPosition() const;
	
	Collider(GameObject * _owner, short mask = 0);

	//Cada tipo de collider se almacena en un set estatico.
	//gracias a esto los metodos estaticos de esta clase hacen
	//de ella el manager de colliders.
	
	static std::set<Collider*> colliders;
	static std::set<Collider*> staticColliders;
	
	virtual bool Collides(Vector2 origin, Vector2 direction, float distance) const = 0;
	virtual bool Collides(const Collider * collider) const = 0;
	virtual bool Collides(const SquareCollider* collider) const = 0;
	virtual bool Collides(const CircleCollider* collider) const = 0;

	static std::list<Collider*> RayCastDynamic(Vector2 origin, Vector2 direction, float distance, int ignoreId);
	static std::list<Collider*> RayCastStatic(Vector2 origin, Vector2 direction, float distance, int ignoreId);
	static std::list<Collider*> RayCastAll(Vector2 origin, Vector2 direction, float distance, int ignoreId);

	static void HandleCollisions();
	
protected:
	static void RayCast(std::list<Collider*> & list, const std::set<Collider*> & collection, Vector2 origin, Vector2 direction, float distance, int ignoreId);

};

class SquareCollider : public Collider
{
public:

	const std::type_info& GetType() override;
	
	SquareCollider(GameObject * _owner, float width, short mask = 0);
	float width;

	bool Collides(Vector2 origin, Vector2 direction, float distance) const override;
	bool Collides(const Collider* collider) const override;
	bool Collides(const SquareCollider* collider) const override;
	bool Collides(const CircleCollider* collider) const override;

};

class CircleCollider : public Collider
{
public:

	const std::type_info& GetType() override;

	CircleCollider(GameObject* _owner, float _radio, short mask = 0);
	float radius;

	bool Collides(Vector2 origin, Vector2 direction, float distance) const override;
	bool Collides(const Collider* collider) const override;
	bool Collides(const SquareCollider* collider) const override;
	bool Collides(const CircleCollider* collider) const override;

};


/*
 *
 *Permite gestionar las fisicas 2D de un
 *objeto dinamico.
 * 
 */

class RigidBody2D : public Component
{
public:
	bool freezePosition;

	const std::type_info& GetType() override;
	
	Vector2 velocity;
	Vector2 lastPosition;

	void UpdatePhysics();
	
	static std::set<RigidBody2D*> rigidBody2D;
	static void ApplyPhysics();
	
	RigidBody2D(GameObject * _owner);
	~RigidBody2D();

	
};


/*
 * En un principio pensamos en crear una clase SpriteRenderer.
 *
 * Sin embargo nos decantamos por tratar el dibujado de manera
 * personalizada en la escena por las dimensiones del proyecto.
 *
 * De todas maneras el dibujado esta desligado de la logica.
 * 
 */
