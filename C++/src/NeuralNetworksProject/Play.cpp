#include "Scene.h"
#include "Collision.h"
#include "Consts.h"
#include "Renderer.h"
#include "Time.h"
#include "AudioManager.h"

#include "Cherry.h"
#include "Strawberry.h"
#include "Orange.h"


Play::Play()
{
	
}


void Play::RunningUpdate(const InputManager &input) {
	
	
	//UPDATE  -------------
	GameObject::UpdateDynamics(input);
	
	//PHYSICS  ---------------
	RigidBody2D::ApplyPhysics();

	//COLLISIONS -------------
	Collider::HandleCollisions();
	
	//HANDLE EVENTS ---------
	GameObject::HandleEvents();

	
}



void Play::Draw() {

	for (const auto& ob : GameObject::drawableObjects)
		ob->Draw();

	hud.Draw();

	for (const auto& canvas : activeCanvas)
		canvas->Draw();
	
}













