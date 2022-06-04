#include "GameData.h"
#include "GameObject.h"

#include "../../dep/inc/XML/rapidxml.hpp"
#include "../../dep/inc/XML/rapidxml_utils.hpp"
#include "../../dep/inc/XML/rapidxml_iterators.hpp"
#include "../../dep/inc/XML/rapidxml_print.hpp"

#include <sstream>

void GameData::SaveRanking(const MatchInfo & info)
{

	std::vector<MatchInfo> current = GetRanking();

	bool found = false;
	for (auto & e : current)
	{
		if (info.name == e.name)
		{
			found = true;
			if (e.points < info.points)
			{
				e.points = info.points;
			}
			break;
		}
	}

	if (!found)
		current.push_back(info);

	std::sort(current.begin(), current.end(), std::greater<MatchInfo>());
	stream_buff::save<MatchInfo>(current, (current.size() % (2 + MAX_RANKING_ROWS)), RANKING_FILE_NAME);

}

std::vector<MatchInfo> GameData::GetRanking()
{
	std::vector<MatchInfo> current;
	stream_buff::recover<MatchInfo>(current, MAX_RANKING_ROWS, RANKING_FILE_NAME);

	return current;
}

void GameData::ImportFromXMLConfigFile(const std::string &xmlConfigFile, std::list<GameObject*> &_walls, std::list<PowerUp*> &_powerUps, Escapist* &_firstPlayer, Player* &_secondPlayer, EnemySpawnManager* &_enemySpawnManager) {
	rapidxml::xml_document<> doc;
	std::ifstream file(xmlConfigFile);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	rapidxml::xml_node<> *gameNode = doc.first_node();
	rapidxml::xml_node<> *positionsNode = gameNode->first_node("Positions");
	rapidxml::xml_node<> *firstPlayerNode = positionsNode->first_node("Player1");
	rapidxml::xml_node<> *secondPlayerNode = positionsNode->first_node("Player2");
	rapidxml::xml_node<> *enemyNode = positionsNode->first_node("Ghost");

	_firstPlayer->playableCharacters[0]->InitPosition({
		(std::atoi(firstPlayerNode->first_attribute("x")->value()) + MAP_TRANSLATE_WIDTH) * SQUARE_WIDTH_WORLD_SPACE,
		(std::atoi(firstPlayerNode->first_attribute("y")->value()) + MAP_TRANSLATE_HEIGHT) * SQUARE_HEIGHT_WORLD_SPACE
		});

	_firstPlayer->playableCharacters[1]->InitPosition({
		(std::atoi(firstPlayerNode->next_sibling()->first_attribute("x")->value()) + MAP_TRANSLATE_WIDTH) * SQUARE_WIDTH_WORLD_SPACE,
		(std::atoi(firstPlayerNode->next_sibling()->first_attribute("y")->value()) + MAP_TRANSLATE_HEIGHT) * SQUARE_HEIGHT_WORLD_SPACE
		});

	_secondPlayer->playableCharacters[0]->InitPosition({
		(std::atoi(secondPlayerNode->first_attribute("x")->value()) + MAP_TRANSLATE_WIDTH) * SQUARE_WIDTH_WORLD_SPACE,
		(std::atoi(secondPlayerNode->first_attribute("y")->value()) + MAP_TRANSLATE_HEIGHT) * SQUARE_HEIGHT_WORLD_SPACE
		});

	_secondPlayer->playableCharacters[1]->InitPosition({
		(std::atoi(secondPlayerNode->next_sibling()->first_attribute("x")->value()) + MAP_TRANSLATE_WIDTH) * SQUARE_WIDTH_WORLD_SPACE,
		(std::atoi(secondPlayerNode->next_sibling()->first_attribute("y")->value()) + MAP_TRANSLATE_HEIGHT) * SQUARE_HEIGHT_WORLD_SPACE
		});

	_enemySpawnManager = new EnemySpawnManager({
		(std::atoi(enemyNode->first_attribute("x")->value()) + MAP_TRANSLATE_WIDTH) * SQUARE_WIDTH_WORLD_SPACE,
		(std::atoi(enemyNode->first_attribute("y")->value()) + MAP_TRANSLATE_HEIGHT) * SQUARE_HEIGHT_WORLD_SPACE
		});

	for (rapidxml::xml_node<> *powerUpNode = positionsNode->first_node("PowerUps")->first_node("Power"); powerUpNode; powerUpNode = powerUpNode->next_sibling()) {
		PowerUp* p = new PowerUp;
		p->InitPosition({
			(std::atoi(powerUpNode->first_attribute("x")->value()) + MAP_TRANSLATE_WIDTH) * SQUARE_WIDTH_WORLD_SPACE,
			(std::atoi(powerUpNode->first_attribute("y")->value()) + MAP_TRANSLATE_HEIGHT) * SQUARE_HEIGHT_WORLD_SPACE
			});

		_powerUps.push_back(p);
	}

	for (rapidxml::xml_node<> *wallNode = gameNode->first_node("Map")->first_node("Wall"); wallNode; wallNode = wallNode->next_sibling()) {
		GameObject* wall = new GameObject(GameObject::Tag::WALL);
		wall->InitPosition({
			(std::atoi(wallNode->first_attribute("x")->value()) + MAP_TRANSLATE_WIDTH) * SQUARE_WIDTH_WORLD_SPACE,
			(std::atoi(wallNode->first_attribute("y")->value()) + MAP_TRANSLATE_HEIGHT) * SQUARE_HEIGHT_WORLD_SPACE
			});
		wall->components.push_back(new SquareCollider(wall, SQUARE_WIDTH_WORLD_SPACE, Collider::MASK_STATIC));

		_walls.push_back(wall);
	}
}

