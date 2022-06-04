#include "Scene.h"
#include "Renderer.h"
#include "GameData.h"

Ranking::Ranking() {
	std::vector<MatchInfo> topPlayers = GameData::GetRanking();

	// init player text
	Renderer* rend = Renderer::Instance();

	Font rankingFont = {
		"rankingFont",
		GAMEPLAY_FONT_PATH,
		RANKING_FONT_SIZE
	};

	rend->LoadFont(rankingFont);

	Vector2 size;
	Vector2 pos;
	for (int index = 0; index < MAX_RANKING_ROWS; index++) {

		Text playerText{
			"playerRanking" + std::to_string(index),
			topPlayers[index].name + " - " + std::to_string(topPlayers[index].points),
			WHITE_COLOR
		};

		rend->LoadTextureText(rankingFont.id, playerText);

		size = rend->GetTextureSize(playerText.id);
		pos.x = (SCREEN_WIDTH / 2.0f) - (size.x / 2);
		pos.y = (SCREEN_HEIGHT / 6.0f) + index * size.y * 2;

		players[index].SetAttributes(rankingFont, playerText, Rect(pos, size));
	}

	// init button menu
	Font rankingButtonFont = {
		"rankingButtonFont",
		PACMAN_FONT_PATH,
		MENU_FONT_SIZE
	};

	rend->LoadFont(rankingButtonFont);

	Text toMenuRankingText{
		"toMenuRanking",
		"MENU",
		RED_COLOR
	};

	Text hoverToMenuRankingText{
		"hoverToMenuRanking",
		"menu",
		RED_COLOR
	};

	rend->LoadTextureText(rankingButtonFont.id, toMenuRankingText);
	rend->LoadTextureText(rankingButtonFont.id, hoverToMenuRankingText);

	menuButton = Button(rankingButtonFont, toMenuRankingText, hoverToMenuRankingText);
	size = rend->GetTextureSize(toMenuRankingText.id);
	pos.x = (SCREEN_WIDTH / 2.0f) - (size.x / 2);
	pos.y = (SCREEN_HEIGHT / 6.0f) * 5;

	menuButton.SetRect(Rect(pos, size));

}

void Ranking::Update(const InputManager &input) {
	menuButton.Update(input);
	if (menuButton.pressed) {
		state = SceneState::ToMenu;
	}
}

void Ranking::Draw() {
	for (auto & player : players) {
		player.Draw();
	}

	menuButton.Draw();
}
