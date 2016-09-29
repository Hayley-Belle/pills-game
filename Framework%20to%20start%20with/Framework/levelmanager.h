#ifndef __LEVELMANAGER_H__
#define __LEVELMANAGER_H__

#include "game.h"

#include <string>
#include <vector>

class LevelManager
{
public:
	typedef enum
	{
		RIGHT,
		LEFT,
		UP,
		DOWN
	} DIRECTION;

	LevelManager();
	~LevelManager();

	bool CreateNewLevel(Game& game);
	bool ProcessValue(Game& game, char c);
	void SetUpLevels();
	bool GetCurrentLevel(DIRECTION D);
	void PlayerDied();

public:
	std::vector<std::string> m_levels;
	std::string m_CurrentLevel;
	float m_xCheckpoint;
	float m_yCheckpoint;

	bool playerCreated;

	int m_xCoord;
	int m_yCoord;

	int m_xLevel;
	int m_yLevel;
};

#endif //__LEVELMANAGER_H__