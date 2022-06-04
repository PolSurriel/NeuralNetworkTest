#include "Player.h"
#include "Types.h"
#include "Fruit.h"
#include "time.h"

Player::Player() {
	for (auto* &playableCharacter : playableCharacters) {
		playableCharacter = new PlayableCharacter;
	}
}


void Player::Update(const InputManager &input) {
	for (auto* &playableCharacter : playableCharacters) {
		playableCharacter->Update(input);
	}
}

void Player::Draw() {
	for (auto* &playableCharacter : playableCharacters) {
		playableCharacter->Draw();
	}
	
}


void Player::SetPlayableCharacterDirectionalKeys(const int &_upKey, const int &_downKey, const int &_leftKey, const int &_rightKey) {
	playableCharacters[0]->SetDirectionalKeys(_upKey, _downKey, _leftKey, _rightKey);
	playableCharacters[1]->SetDirectionalKeys(_downKey, _upKey, _rightKey, _leftKey);
}

Player::~Player() {
	
	for (auto* &playableCharacter : playableCharacters) {
		delete playableCharacter;
	}
	
}

Escapist::Escapist() {
	for (auto* &playableCharacter : playableCharacters) {
		playableCharacter->type = PlayableCharacter::Type::ESCAPIST;
	}
	secondsPlaying = 0;

	lives = ESCAPIST_LIVES;
	points = 0;
}


int Escapist::GetPoints() {
	return points;
}

void Escapist::AddPoints(const int &_pointsToAdd) {
	int totalPoints = points + _pointsToAdd;
	if (totalPoints > MAX_PLAYER_POINTS) totalPoints = MAX_PLAYER_POINTS;
	
	points = totalPoints;
}


void Escapist::Update(const InputManager& input)
{
	float lastSecond = trunc(secondsPlaying);
	secondsPlaying += FPSTime::deltaTimeInMilliseconds / 1000;
	
	if (lastSecond != trunc(secondsPlaying)) {
		AddPoints(1);
	}
	
}

void Escapist::HandleEvent(std::pair<Event, stream_buff::buffer> evt_info)
{

	switch (evt_info.first)
	{
	case Event::FRUIT_GOTTEN:
		
		AddPoints(
			stream_buff::get<int>(evt_info.second, 0)
		);
		break;

	case Event::MAINPLAYER_GETS_DAMAGE:
		lives--;
		break;

	case Event::POWER_UP_GOTTEN:
		for (auto & e : playableCharacters)
			e->Empower();
		break;
	case Event::ENEMY_KILLED:
		AddPoints(POINTS_ON_KILL_ENEMY);
		
		break;
	}
}

