#pragma once
#include "PlayableCharacter.h"
#include "Consts.h"
#include "Types.h"


class Player : public GameObject {
public:
	PlayableCharacter* playableCharacters[PLAYABLE_CHARACTERS_FOR_EACH_PLAYER];

	void SetPlayableCharacterDirectionalKeys(const int &_upKey, const int &_downKey, const int &_leftKey, const int &_rightKey);
	void Update(const InputManager &input);
	void Draw();

	Player();
	~Player();
};

class Persecutor : public Player {
	// ?
};

class Escapist : public Player {
	
	int points;

public:

	float secondsPlaying;
	
	int lives;

	Escapist();
	void InitStats();
	int GetPoints();
	void AddPoints(const int &_pointsToAdd);

	void Update(const InputManager& input) override;

	void HandleEvent(std::pair<Event, stream_buff::buffer> evt_info) override;
};