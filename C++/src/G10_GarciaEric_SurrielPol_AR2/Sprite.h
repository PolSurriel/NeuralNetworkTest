#pragma once
#include <string>
#include <unordered_map>

#include "Types.h"

class Sprite {
public:
	std::string spriteId;
	Rect texture;

	Sprite();
	Sprite(std::string _spriteId, Rect _texture);
};

class AnimatedSprite : public Sprite {
private:

	std::unordered_map<int, std::pair<std::string, Rect>> extraTextures;

public:
	enum class Type{
		UP, DOWN, RIGHT, LEFT, DISINTEGRATION
	};

	AnimatedSprite();
	AnimatedSprite(std::string _spriteId, Rect _texture);
	void SetTexture(Type type, int state);
	void SetTexture(int key);
	void PushTexture(int key, std::string extraSpriteId, Rect extraTexture);
};