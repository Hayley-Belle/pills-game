#include "levelmanager.h"
#include "logmanager.h"

#include <fstream>
#include <algorithm>

LevelManager::LevelManager()
: m_levels(0)
, m_CurrentLevel("")
, playerCreated(0)
{
	m_xCoord = 0;
	m_yCoord = 0;

	m_xLevel = 0;
	m_yLevel = 0;
}

LevelManager::~LevelManager()
{
	
}

void
LevelManager::SetUpLevels()
{
	m_levels.push_back("assets\\levels\\level01.txt");
	m_levels.push_back("assets\\levels\\level02.txt");
	m_levels.push_back("assets\\levels\\level03.txt");
	m_levels.push_back("assets\\levels\\level04.txt");
	m_levels.push_back("assets\\levels\\level05.txt");
	m_levels.push_back("assets\\levels\\level06.txt");
	m_levels.push_back("assets\\levels\\level07.txt");
	m_CurrentLevel = m_levels[0];
}

bool
LevelManager::GetCurrentLevel(DIRECTION d)
{
	switch (d)
	{
	case RIGHT: ++m_xLevel; break;
	case LEFT: --m_xLevel; break;
	case UP: ++m_yLevel; break;
	case DOWN: --m_yLevel; break;
	}

	std::string line;
	for (unsigned int i = 0; i < m_levels.size(); ++i)
	{
		std::ifstream myFile;
		
		myFile.open(m_levels[i]);
		if (myFile.is_open())
		{
			bool start = false;
			bool running = true;
			while (std::getline(myFile, line) && running)
			{
				if (start)
				{
					// Remove spaces
					line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
					
					int x = line[2] - '0';
					std::getline(myFile, line);
					int y = line[2] - '0';
					if (x == m_xLevel && y == m_yLevel)
					{
						m_CurrentLevel = m_levels[i];
						return true;
					}
					running = false;
				}
				if (line.compare(":ID:") == 0)
				{
					start = true;
				}
			}
		}
		else
		{
			LogManager::GetInstance().Log("Failed to open this file");
		}

	}
	switch (d)
	{
	case RIGHT: --m_xLevel; break;
	case LEFT: ++m_xLevel; break;
	case UP: --m_yLevel; break;
	case DOWN: ++m_yLevel; break;
	}

	return false;
}

bool
LevelManager::CreateNewLevel(Game& game)
{
	m_yCoord = 0;
	std::ifstream myFile;
	std::string line;
	myFile.open(m_CurrentLevel);
	if (myFile.is_open())
	{
		bool start = false;

		LogManager::GetInstance().Log("Level file has been opened succesfully");
		while (std::getline(myFile, line))
		{
			
			if (line.compare(":/Map:") == 0)
			{
				start = false;
			}
			if (start)
			{
				// Remove spaces
				line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); 
				
				// Checks for appropriate length
				if (line.size() != 16) // 800(screen size) / 50(tile size) = 16
				{
					LogManager::GetInstance().Log("WARNING: Level data should be 16 x 12\nSome tiles may not be loaded!");
				}
				else
				{
					// Reset X counter
					m_xCoord = 0;
					for (unsigned int i = 0; i < line.size(); ++i)
					{
						if (!ProcessValue(game, line[i]))
						{
						}
						++m_xCoord;
					}
				}
				++m_yCoord;
			}
			if (line.compare(":Map:") == 0)
			{
				start = true;
			}
		}
	}
	else
	{
		LogManager::GetInstance().Log("Failed to load level");
	}

	return false;
}

bool
LevelManager::ProcessValue(Game& game, char c)
{
	bool result = true;
	switch (c)
	{
	case '0':
		break;
	case '1':
		game.SpawnTile(m_xCoord * 50, m_yCoord * 50, Tile::Type::GRASS);
		break;
	case '2':
		game.SpawnTile(m_xCoord * 50, m_yCoord * 50, Tile::Type::BOUNCE);
		break;
	case '3':
		game.SpawnTile(m_xCoord * 50, m_yCoord * 50, Tile::Type::DAMAGE);
		break;
	case 'A':
		game.SpawnTile(m_xCoord * 50, m_yCoord * 50, Tile::Type::ANCHOR);
		break;
	case 'P':
		if (!playerCreated)
		{
			game.SpawnPlayer((m_xCoord * 50) + 5, m_yCoord * 50);
			playerCreated = true;
			m_xCheckpoint = ((float)m_xCoord * 50) + 5;
			m_yCheckpoint = (float)m_yCoord * 50;
		}
		else
		{
			game.UpdatePlayerPosition((m_xCoord * 50) + 5, m_yCoord * 50);
		}
		
		break;
	case 'E':
		game.SpawnEnemy(m_xCoord * 50, m_yCoord * 50);
		break;
	default: 
		result = false;
		break;
	}
	return result;
}

void
LevelManager::PlayerDied()
{
	m_CurrentLevel = m_levels[0];
	m_xLevel = 0;
	m_yLevel = 0;
}
