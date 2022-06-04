#pragma once
#include "Types.h"

//Game general information
const int SCREEN_WIDTH{ 900 };
const int SCREEN_HEIGHT{ 700 };

const float TITLE_WIDTH{ 70.0f };
const float TITLE_HEIGHT{ 37.8f };

const int SPLASH_TITLE_FONT_SIZE{ 120 };
const int SPLASH_SUBTITLE_FONT_SIZE{ 50 };
const int MENU_FONT_SIZE{ 80 };
const int HUD_FONT_SIZE{ 60 };
const int RANKING_FONT_SIZE{ 35 };
const int START_GAME_FONT_SIZE{ 45 };
const int STOP_GAME_FONT_SIZE{ 100 };

const int BUTTON_MENU_COUNT{ 4 };

const int PLAYABLE_CHARACTERS_FOR_EACH_PLAYER{ 2 };
const int FPS{ 60 };

const float SQUARE_WIDTH_WORLD_SPACE{ 35.0f };
const float SQUARE_HEIGHT_WORLD_SPACE{ 35.0f };
const float SQUARE_RADIO_WORLD_SPACE{ 17.5f };

const int MAP_TRANSLATE_HEIGHT{ -1 };
const int MAP_TRANSLATE_WIDTH{ -1 };

const int ESCAPIST_LIVES{ 3 };

const float MAP_WIDTH{ SQUARE_WIDTH_WORLD_SPACE * 20 };
const float MAP_HEIGHT{ SQUARE_HEIGHT_WORLD_SPACE * 20 };

const int HUD_WIDTH{ 200 };
const int HUD_HEIGHT{ 700 };

const float SECONDS_SPAWN_FRUIT{ 10.0f };
const float SECONDS_RESPAWN_FRUIT{ 15.0f };
const float SECONDS_RESPAWN_FRUIT_IF_COLLISION{ 2.0f };

const int POINTS_ON_KILL_ENEMY{ 15 };

const bool PRINT_FPS{ false };

const float ENTITY_DEFAULT_SPEED{ 1.0f / 6.0f };

const int MAX_PLAYER_POINTS{ 9999 };
const int MAX_PLAYER_FRUIT_COUNT{ 9 };

const int MAX_FRUIT_SPAWNED_STREAK{ 3 };

const int CHERRY_POINTS_VALUE{ 10 };
const int STRAWBERRY_POINTS_VALUE{ 15 };
const int ORANGE_POINTS_VALUE{ 20 };

const float DISTANCE_BETWEEN_WALL_AND_PACMAN_RD{ 2.7f };
const float DISTANCE_BETWEEN_WALL_AND_PACMAN_UL{ 2.8f };

const Vector2 INITIAL_POSITION{ 0, 0 };
const Vector2 TITLE_POSITION{ 100, 50 };
const Vector2 PLAY_BUTTON_POSITION{ 100, 450 };
const Vector2 SOUND_BUTTON_POSITION{ 275, 450 };
const Vector2 EXIT_BUTTON_POSITION{ 600, 450 };

const Vector2 VECTOR2_ZERO{ 0, 0 };

const Vector2 VECTOR2_UP{ 0, -1 };
const Vector2 VECTOR2_DOWN{ 0, 1 };
const Vector2 VECTOR2_LEFT{ -1, 0 };
const Vector2 VECTOR2_RIGHT{ 1, 0 };

const Vector2 ENEMY_INITIAL_DIRECTION{ VECTOR2_LEFT };

const color WHITE_COLOR{ 255,255,255,0 };
const color RED_COLOR{ 255,0,0,0 };
const color BLACK_COLOR{ 0,0,0,0 };
const color ORANGE_COLOR{ 255, 204, 0,0 };

const int MILLISECONDS_TO_UPDATE_SPRITE{ 250 };
const int NUM_SPRITE_DIRECTION_FRAMES{ 2 };
const int NUM_SPRITE_DISINTEGRATION_FRAMES{ 12 };
const int NUM_SPRITE_GHOST_EXTRA_FRAMES{ 4 };
const int MILLISECONDS_DISINTEGRATING{ MILLISECONDS_TO_UPDATE_SPRITE * NUM_SPRITE_DISINTEGRATION_FRAMES };

const int RUN_TEXT_CANVAS_LINES{ 2 };

const std::string FRUIT_INIT_HUD_TEXT{ "X 0" };

const std::string PACMAN_SPRITE_SHEET_ID{ "pacManSpriteSheet" };
const std::string PACMAN_SPRITE_SHEET_PATH{ "../../res/img/PacManSpritesheet.png" };
const std::string ENEMY_SPRITE_SHEET_ID{ "ghostSpriteSheet" };
const std::string ENEMY_SPRITE_SHEET_PATH{ "../../res/img/ghost_blinking.png" };

const std::string PACMAN_FONT_PATH{ "../../res/ttf/PAC-FONT.TTF" };
const std::string GAMEPLAY_FONT_PATH{ "../../res/ttf/Gameplay.ttf" };

const std::string PACMAN_INTRO_THEME_PATH{ "../../res/au/pacmanBeginning.wav" };
const std::string PACMAN_INTRO_THEME_NAME{ "pacman" };
const std::string MENU_THEME_PATH{ "../../res/au/menu.mp3" };
const std::string MENU_THEME_NAME{ "menu" };
const std::string GAME_THEME_PATH{ "../../res/au/game_theme.mp3" };
const std::string GAME_THEME_NAME{ "bomberman" };
const std::string BATMAN_LOOP_THEME_PATH{ "../../res/au/batmanIntroTheme.mp3" };
const std::string BATMAN_LOOP_THEME_NAME{ "batman" };

const std::string XML_CONFIG_FILE{ "../../res/files/config_pc.xml" };

const std::string RANKING_FILE_NAME{ "../../res/files/ranking.bin" };
const int MAX_RANKING_ROWS{ 5 };

