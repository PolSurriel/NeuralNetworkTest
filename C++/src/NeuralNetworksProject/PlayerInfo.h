#pragma once
#include "Types.h"

class PlayerInfo
{
	Rect rect;
	Font font;

	Text playerName;

public:
	PlayerInfo();
	void SetAttributes(const Font &_font, const Text &_playerName, const Rect &_rect);

	void Draw();
};