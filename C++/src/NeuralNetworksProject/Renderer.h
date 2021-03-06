#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string.h>
#include <unordered_map>
#include "Types.h"
#undef main

class Renderer {

private:
	std::unordered_map<std::string, SDL_Texture*> m_textureData;
	std::unordered_map<std::string, TTF_Font*> m_fontData;
	static Renderer *renderer;
	Renderer();

	SDL_Rect RectToSDLRect(const Rect &r);

public:
	SDL_Window *m_window = nullptr;
	SDL_Renderer *m_renderer = nullptr;


	static Renderer *Instance()
	{
		if (renderer == nullptr) { renderer = new Renderer; }
		return renderer;
	};

	~Renderer();
	void DeleteResources();
	void Clear();
	void Render();
	void LoadFont(Font font);
	void LoadTexture(const std::string &id, const std::string &path);
	void LoadTextureText(const std::string &fontId, const Text &text);
	Vector2 GetTextureSize(const std::string &id);
	void PushImage(const std::string &id, const Rect &rect);
	void PushSprite(const std::string &id, const Rect &rectSprite, const Rect &rectPos);
	void PushRotatedSprite(const std::string &id, const Rect &rectSprite, const Rect &rectPos, float angle);
	void SetRenderDrawColor(int r, int g, int b);
};
