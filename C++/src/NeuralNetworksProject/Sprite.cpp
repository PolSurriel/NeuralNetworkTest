#include "Sprite.h"
#include "Consts.h"

Sprite::Sprite() {}
Sprite::Sprite(std::string _spriteId, Rect _texture) : spriteId(_spriteId), texture(_texture) {}

AnimatedSprite::AnimatedSprite() {}
AnimatedSprite::AnimatedSprite(std::string _spriteId, Rect _texture) {
	spriteId = _spriteId;
	texture = _texture;
	PushTexture(0, _spriteId, _texture);
}

void AnimatedSprite::SetTexture(Type type, int state) {
	std::pair<std::string, Rect>* extraTexture = &extraTextures[((int)type) * NUM_SPRITE_DIRECTION_FRAMES + state];

	spriteId = extraTexture->first;
	texture = extraTexture->second;
}

void AnimatedSprite::SetTexture(int key) {
	texture = extraTextures[key].second;
}

void AnimatedSprite::PushTexture(int key, std::string extraSpriteId, Rect extraTexture) {
	extraTextures[key] = { extraSpriteId, extraTexture };
}