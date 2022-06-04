#include "PowerUp.h"
#include "Consts.h"
#include "PlayableCharacter.h"

PowerUp::PowerUp()
{
	tag = Tag::POWER_UP;
	components.push_back(new SquareCollider(this, SQUARE_WIDTH_WORLD_SPACE));

	
}

PowerUp::PowerUp(int _points) : points(_points) {
	tag = Tag::POWER_UP;
};


void PowerUp::Update(const InputManager& input)
{
	
}

void PowerUp::OnCollision(Collider &_collider)
{
	switch (_collider.gameObject->tag)
	{
	case GameObject::Tag::PLAYABLE_CHARACTER:

		if( ((PlayableCharacter*)_collider.gameObject)->type  == PlayableCharacter::Type::ESCAPIST)
		{
			GameObject::Destroy(this);
		}
		
		break;
	}
}

void PowerUp::HandleEvent(std::pair<Event, stream_buff::buffer> evt_info)
{
	
}




//void PowerUp::Update(InputManager input) {
//
//}
//
//void PowerUp::Draw() {
//
//}