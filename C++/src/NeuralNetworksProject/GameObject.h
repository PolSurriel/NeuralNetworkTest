#pragma once
#include "Sprite.h"
#include <list>
#include <stack>
#include "StreamBuffer.h"
#include "Types.h"
#include "Component.h"
#include "InputManager.h"


/*
 *
 * Las clases GameObject y Component deben agregarse mútuamente.
 * (Aunque dado que component no tiene sentido fuera de gameobject su relación es de composición)
 *
 * "gameObject.GetComponent<RigidBody2D>().gameObject"
 *
 * Los componentes deben poder comunicarse entre sí y acceder a los datos de su propietario (GameObject)
 *
 * Somos conscientes de que estas relaciones no son una buena praxis salvo en ocasiones contadas (como esta).
 *
 * Antes de tomar la decision de implementarlo hemos leido el codigo fuente de Unity para confirmar que
 * así es como esta implementado.
 *
 * Hemos buscado entre diferentes posts como han resuelto otros developers este problema
 * y en todos los casos se emplea la misma estrategia.
 *
 * La mejor referencia que hemos encontrado ha sido la respuesta del usuario TOM__ en el siguiente post de stackoverflow:
 * https://stackoverflow.com/questions/44105058/how-does-unitys-getcomponent-work
 *
 * También hemos investigado acerca del patron de diseño "visitor"
 * https://cpppatterns.com/patterns/visitor.html
 *
 * [/Easter egg]
 * https://subefotos.com/ver/?8a9338573ce3244781eb33be289b43d4o.jpg
 *
 */

// Declaramos sin definir las clases de componentes para que pueda producirse la agregacion bidireccional.
class Component;
class Collider;
class RigidBody2D;
class SpriteRenderer;
class SquareCollider;


typedef short GameObjectMask;


class GameObject
{
	static int lastId;
	int id;

public:

	/*
	 * Hemos decidido que los eventos de gameobject se encuentren dentro de su clase
	 * porque son eventos DE game object y esta comunicación es exclusiva de gameobjects.
	 *
	 */
	
	enum class Event
	{
		MAINPLAYER_GETS_DAMAGE,
		FRUIT_GOTTEN,
		POWER_UP_GOTTEN,
		ENEMY_KILLED,
		POWER_UP_TIME_OVER,
		RESET_POSITIONS
	};

	static const GameObjectMask DYNAMIC = 1; //0001
	static const GameObjectMask DRAWABLE = 2; //0010
	static const GameObjectMask DYNAMIC_DRAWABLE = 3; //0011
	
	
	static std::set<GameObject*> dynamicGameObjects;
	static std::set<GameObject*> drawableObjects;


	/*
	 *Nota:
	 *No queremos que los eventos se ejecuten de manera inmediata
	 *porque pueden afectar al comportamiento durante la gestion
	 *de colisiones. De lo contrario haríamos el foreach en cada
	 *throw.
	 *
	 *Por ello lo que hacemos es "stackear" los eventos para
	 *lanzarlos en un momento determinado del flujo.
	 *
	 *
	 *En un principio pensamos en hacer una tabla de suscripcion
	 *con un map. De esta manera nunca se llamaria al handle event
	 *de un objeto que no se haya suscrito a el. Cosa que ahorraria
	 *muchas llamadas a funcion innecesarias.
	 *
	 *No lo hemos hecho porque mantener estas colecciones actualizadas
	 *de manera eficiente implicaria guardarse los accesos y no queriamos
	 *complicar mas la estructura del codigo para unos requisitos de la
	 *entrega que no lo necesitan.
	 *
	 *	Ejemplo:
	 *
	 *   ._______________________________________.
	 *   |___________________Map_________________|
	 *   |__KEY__|___________SET_________________|
	 *   |EVENTO1|Object*|Object*|Object*|Object*|
	 *   |EVENTO2|Object*|Object*|Object*|Object*|
	 *   |EVENTO3|Object*|Object*|Object*|Object*|
	 *   |EVENTO4|Object*|Object*|Object*|Object*|
	 *   |EVENTO5|Object*|Object*|Object*|Object*|
	 *   ·---------------------------------------·
	 *
	 *	Cada game object al destruirse debería "desuscribirse" de la tabla.
	 *
	 *	Es decir, for each eventoSuscrito erease(this).
	 *
	 *	Asi es como lo planteariamos de escalar el proyecto.
	 *
	 *   
	 */   
	
	static std::stack<std::pair<Event, stream_buff::buffer>> eventsThrown;
	static void Throw(Event evt, stream_buff::buffer buff = nullptr);
	
	static std::stack<GameObject*> toDestroy;
	static std::stack<std::pair<GameObjectMask, GameObject*>> toCreate;

	static void Instantiate(GameObject* gameObject, GameObjectMask mask = 0);

	static void HandleEvents();
	static void UpdateDynamics(const InputManager & input);

	static void ManageInstances();
	
	// Los tags estan pensados como algo que
	// desde una interfaz visual se asociaria a
	// un string pero que desde codigo compilado
	// son enums.
	enum class Tag {
		DEFAULT, BUTTON, SWITCH_BUTTON, POWER_UP, STRAWBERRY, CHERRY, ORANGE, ENEMY, PLAYABLE_CHARACTER, WALL
	};

	static void Destroy(GameObject * gameObject);
	
	Tag tag;

	int GetId();

	std::list<Component*> components;

	template <class T>
	T* GetComponent();

	Vector2 position;
	Sprite* actualSprite;

	virtual void HandleEvent(std::pair<Event, stream_buff::buffer> evt_info);
	
	virtual void OnCollision(Collider &_collider);
	virtual void InitPosition(Vector2 _position);
	virtual void Draw();

	virtual void Update(const InputManager& input);
	
	GameObject(short mask = 0);
	GameObject(Tag _tag);
	virtual void SetSprite(Sprite* &sprite);
	~GameObject();

};

template <class T>
T* GameObject::GetComponent()
{
	for(auto & e: components)
	{
		if (typeid(T) == e->GetType())
		{
			return (T*)e;
		}
	}



	return nullptr;

}

