#include "PlayerInfo.h"
#include "Renderer.h"

PlayerInfo::PlayerInfo() {}

void PlayerInfo::SetAttributes(const Font &_font, const Text &_playerName, const Rect &_rect) {
	font = _font;
	playerName = _playerName;
	rect = _rect;
}

void PlayerInfo::Draw() {
	Renderer::Instance()->PushImage(
		playerName.id,
		rect
	);
}