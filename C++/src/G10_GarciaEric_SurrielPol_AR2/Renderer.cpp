#include "Renderer.h"
#include "Types.h"
#include "Consts.h"

Renderer::Renderer()
{
	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";

	// --- WINDOW ---
	m_window = SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (m_window == nullptr) throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";

	//Initialize renderer color
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	//Initialize PNG loading
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_imageinit";

	// ---- TTF ----
	if (TTF_Init() != 0) throw"No es pot inicialitzar SDL_ttf";

};


Renderer::~Renderer()
{
	DeleteResources();
	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;
	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
};

void Renderer::DeleteResources() {
	for (auto &t : m_textureData) SDL_DestroyTexture(t.second), t.second = nullptr;
	for (auto &f : m_fontData) TTF_CloseFont(f.second), f.second = nullptr;
}

SDL_Rect Renderer::RectToSDLRect(const Rect &r) {
	return { r.x, r.y, r.w, r.h };
}

void Renderer::Clear() { SDL_RenderClear(m_renderer); };

void Renderer::Render() { SDL_RenderPresent(m_renderer); };

void Renderer::LoadFont(Font font) {
	TTF_Font *ttfFont{ TTF_OpenFont(font.path.c_str(), font.size) };
	if (ttfFont == nullptr) throw "No es pot inicialitzar TTF_Font";
	m_fontData[font.id] = ttfFont;
};

void Renderer::LoadTexture(const std::string &id, const std::string &path) {


	SDL_Texture *texture{ IMG_LoadTexture(m_renderer, path.c_str()) };
	if (texture == nullptr) throw "No s'han pogut crear les textures";
	m_textureData[id] = texture;
};

void Renderer::LoadTextureText(const std::string &fontId, const Text &text) {	
	SDL_Surface	*tmpSurf = TTF_RenderText_Blended(
		m_fontData[fontId], 
		text.text.c_str(), 
		SDL_Color{ static_cast<Uint8>(text.color.r),
			static_cast<Uint8>(text.color.g),
			static_cast<Uint8>(text.color.b),
			static_cast<Uint8>(text.color.a)}
	);

	if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
	SDL_Texture *texture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	m_textureData[text.id] = texture;
};

Vector2 Renderer::GetTextureSize(const std::string &id) {
	int w, h;
	SDL_QueryTexture(m_textureData[id], NULL, NULL, &w, &h);
	return { static_cast<float>(w), static_cast<float>(h) };
};

void Renderer::PushImage(const std::string &id, const Rect &rect) {
	
	//****
	auto p1 = RectToSDLRect(rect);
	SDL_RenderCopy(m_renderer, m_textureData[id], nullptr, &p1);
};

void Renderer::PushSprite(const std::string &id, const Rect &rectSprite, const Rect &rectPos) {
	//****
	auto p1 = RectToSDLRect(rectSprite);
	auto p2 = RectToSDLRect(rectPos);

	SDL_RenderCopy(m_renderer, m_textureData[id], &p1, &p2);

}

void Renderer::PushRotatedSprite(const std::string & id, const Rect & rectSprite, const Rect & rectPos, float angle) {
	SDL_Point center = { rectPos.w / 2, rectPos.h / 2 };
	//*****
	auto p1 = RectToSDLRect(rectSprite);
	auto p2 = RectToSDLRect(rectPos);
	auto p3 = &center;
	SDL_RenderCopyEx(m_renderer, m_textureData[id], &p1, &p2, angle, p3, SDL_FLIP_NONE);
}

void Renderer::SetRenderDrawColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
}

Renderer* Renderer::renderer = nullptr;
