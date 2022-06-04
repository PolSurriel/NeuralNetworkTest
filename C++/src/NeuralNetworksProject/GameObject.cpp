#include "GameObject.h"
#include "Renderer.h"
#include "Consts.h"

int GameObject::lastId = 0;

GameObject::GameObject(short mask)
{
	id = GameObject::lastId++;

	if (mask & DYNAMIC)
		dynamicGameObjects.insert(this);
	
}

GameObject::GameObject(Tag _tag) : tag(_tag) { id = GameObject::lastId++; }

std::stack<std::pair<GameObject::Event, stream_buff::buffer>> GameObject::eventsThrown;

std::stack<GameObject*> GameObject::toDestroy;
std::stack<std::pair<GameObjectMask, GameObject*>> GameObject::toCreate;

std::set<GameObject*> GameObject::dynamicGameObjects;
std::set<GameObject*> GameObject::drawableObjects;


void GameObject::SetSprite(Sprite* &sprite) {
	actualSprite = sprite;
}

void GameObject::OnCollision(Collider &_collider) {}

void GameObject::InitPosition(Vector2 _position) {
	position = _position;
}

void GameObject::Draw() {
	Renderer::Instance()->PushSprite(actualSprite->spriteId,
		actualSprite->texture,
		{ static_cast<int>(position.x),static_cast<int>(position.y), static_cast<int>(SQUARE_WIDTH_WORLD_SPACE), static_cast<int>(SQUARE_HEIGHT_WORLD_SPACE) }
	);
}

int GameObject::GetId()
{
	return id;
}

GameObject::~GameObject() {

	for (auto &component : components) {
		delete component;
	}
}

void GameObject::HandleEvent(std::pair<Event, stream_buff::buffer> evt_info)
{
	
}

void GameObject::Throw(Event evt, stream_buff::buffer buff)
{
	
	eventsThrown.push(std::pair<GameObject::Event, stream_buff::buffer>(evt, buff));
}

void GameObject::ManageInstances()
{
	while (!GameObject::toDestroy.empty())
	{
		GameObject::dynamicGameObjects.erase(GameObject::toDestroy.top());
		drawableObjects.erase(GameObject::toDestroy.top());
		
		delete GameObject::toDestroy.top();
		GameObject::toDestroy.top() = nullptr;

		GameObject::toDestroy.pop();
	}

	while (!GameObject::toCreate.empty())
	{
		auto toCreate = GameObject::toCreate.top();

		if (toCreate.first & GameObject::DYNAMIC)
			GameObject::dynamicGameObjects.insert(toCreate.second);

		if (toCreate.first & GameObject::DRAWABLE)
			GameObject::drawableObjects.insert(toCreate.second);

		GameObject::toCreate.pop();
	}
}

void GameObject::UpdateDynamics(const InputManager & input)
{
	ManageInstances();
	
	for (auto & gameObject : GameObject::dynamicGameObjects)
	{
		if(gameObject == nullptr)
		{
			dynamicGameObjects.erase(gameObject);
			drawableObjects.erase(gameObject);
			
		}else
		{
			gameObject->Update(input);
			
		}
		
	}

}



void GameObject::Update(const InputManager& input)
{
	
}


void GameObject::Destroy(GameObject* gameObject)
{
	GameObject::toDestroy.push(gameObject);
}


void GameObject::Instantiate(GameObject* gameObject, GameObjectMask mask)
{
	toCreate.push(std::pair<GameObjectMask, GameObject*>(mask, gameObject));
}


void GameObject::HandleEvents()
{
	
	while (!GameObject::eventsThrown.empty())
	{
		std::pair<GameObject::Event, stream_buff::buffer> evt = GameObject::eventsThrown.top();

		for (auto & gameObject : dynamicGameObjects)
			gameObject->HandleEvent(evt);

		delete evt.second;

		GameObject::eventsThrown.pop();
	}

}
