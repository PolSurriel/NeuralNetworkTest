#pragma once
#include "GameObject.h"

class PowerUp : public GameObject
{
public:

	int points;
	
	PowerUp();
	PowerUp(int _points);

	void Update(const InputManager& input) override;

	void OnCollision(Collider &_collider);

	void HandleEvent(std::pair<Event, stream_buff::buffer> evt_info) override;
	
/*
	void Update(InputManager input);
	void Draw();*/
};

