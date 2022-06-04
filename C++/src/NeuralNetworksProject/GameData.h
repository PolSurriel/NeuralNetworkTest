#pragma once
#include <string>
#include <algorithm>
#include "StreamBuffer.h"
#include "Consts.h"
#include "PowerUp.h"
#include "Player.h"
#include "Enemy.h"


/*Game data es la clase que ofrece indireccion
 *respecto a streamBuffer (que a su misma vez ofece
 *indireccion respecto a fstream.
 *
 *Tambien lee los ficheros XML
 */

struct MatchInfo
{
	int points;
	std::string name;

	bool operator >(const MatchInfo &m) const
	{
		return points > m.points;
	}
};



class GameData
{
public:
	static void SaveRanking(const MatchInfo & info);
	static std::vector<MatchInfo> GetRanking();
	static void ImportFromXMLConfigFile(const std::string &xmlConfigFile, std::list<GameObject*> &_walls, std::list<PowerUp*> &_powerUps, Escapist* &_firstPlayer, Player* &_secondPlayer, EnemySpawnManager* &_enemySpawnManager);
	
};

