// COMP710 GP 2D Framework
#ifndef __GAME_H__
#define __GAME_H__

#include "tile.h"

#include <vector>

// TODO: Clean this up

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class AnimatedSprite;
class Player;
class Enemy;
class LevelManager;
class Menu;

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void MovePlayerLeft();
	void MovePlayerRight();
	void PlayerJump(float height);
	void StopMovement();

	void FireSpaceShipBullet();

	void MouseClick(int x, int y);
	void MouseReleased();

	void SpawnPlayer(int x, int y);
	void SpawnEnemy(int x, int y);
	void SpawnExplosion(int x, int y);
	
	void SpawnTile(int x, int y, Tile::Type type);

	void ResetPlayer(int width, int height);

	void ProcessEnemyMovement(float deltaTime);

	void UpdatePlayerPosition(int x, int y);
	void ReloadLevel();

	void UpdateMenu(int x, int y);

	enum GameState
	{
		TITLE,
		PLAY,
		INSTRUCTIONS,
		CREDITS,
		PAUSE,
		WIN,
		LOSE,
		EXIT
	};

	inline GameState GetGameState(){ return m_currentGameState; }
	void SetGameState(GameState toSet);
	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	LevelManager* m_pLevelManager;
	Menu* m_pMenu;
	GameState m_currentGameState;

	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

	// Game Entities:
	// W02.1: Add a PlayerShip field.     DONE
	Player* m_pPlayer;
	std::vector<Tile*> grass;
	std::vector<Enemy*> enemies;

	// W02.2: Add an alien enemy container field.
	// W02.3: Add a bullet container field.

private:

};

#endif // __GAME_H__
