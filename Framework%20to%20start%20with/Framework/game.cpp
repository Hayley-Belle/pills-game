// COMP710 GP 2D Framework

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "player.h"
#include "tile.h"
#include "enemy.h"
#include "levelmanager.h"
#include "animatedsprite.h"
#include "menu.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <algorithm>

// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
	: m_pBackBuffer(0)
	, m_pInputHandler(0)
	, m_looping(true)
	, m_executionTime(0)
	, m_elapsedSeconds(0)
	, m_frameCount(0)
	, m_FPS(0)
	, m_numUpdates(0)
	, m_lastTime(0)
	, m_lag(0)
	, grass(0)
	, enemies(0)
	, m_pLevelManager(0)
	, m_pMenu(0)
{
	
}

Game::~Game()
{
	delete m_pPlayer;
	m_pPlayer = 0;

	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_pInputHandler;
	m_pInputHandler = 0;

	delete m_pLevelManager;
	m_pLevelManager = 0;

	delete m_pMenu;
	m_pMenu = 0;

	for (unsigned int i = 0; i < grass.size(); ++i)
	{
		delete grass[i];
	}
	grass.clear();

	for (unsigned int i = 0; i < enemies.size(); ++i)
	{
		delete enemies[i];
	}
	enemies.clear();
}

bool 
Game::Initialise()
{
	const int width = 800;
	const int height = 600;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	m_currentGameState = TITLE;
	m_pMenu = new Menu();
	m_pMenu->Initialise(m_pBackBuffer);
	
	m_pLevelManager = new LevelManager();
	m_pLevelManager->SetUpLevels();
	m_pLevelManager->CreateNewLevel(*this);

	// Could remove grass processing: as they remain static
	for (unsigned int i = 0; i < grass.size(); ++i)
	{
		grass[i]->Process(0);
	}

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	// Processes each of the key presses
	m_pInputHandler->ProcessInput(*this);
	
	if (m_looping)
	{
		
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;

		switch (m_currentGameState)
		{
		case TITLE:
			m_pMenu->Draw(m_pBackBuffer);
			break;

		case PLAY: 
			m_lastTime = current;

			m_executionTime += deltaTime;

			m_lag += deltaTime;

			while (m_lag >= stepSize)
			{
				Process(stepSize); // TODO : Check timestep with process(0)

				m_lag -= stepSize;

				++m_numUpdates;
			}

			Draw(*m_pBackBuffer);
			break;

		case INSTRUCTIONS:
			m_pMenu->Draw(m_pBackBuffer);
			break;

		case CREDITS:
			m_pMenu->Draw(m_pBackBuffer);
			break;

		case PAUSE:
			current = m_lastTime;
			m_pMenu->Draw(m_pBackBuffer);
			break;

		case EXIT:
			Quit();
			break;
		}
	}

	SDL_Delay(1);


	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	// Process background movement
	m_pBackBuffer->Process(100* deltaTime);
	
	// Could remove grass processing: as they remain static
	for (unsigned int i = 0; i < grass.size(); ++i)
	{
		grass[i]->Process(0);
	}

	// Process enemy movements
	ProcessEnemyMovement(deltaTime);

	// W02.1: Update player... DONE
	m_pPlayer->Process(deltaTime); 

	bool reloadLevel = false;
	if (m_pPlayer->GetPositionX() < 0)
	{
		if (m_pLevelManager->GetCurrentLevel(LevelManager::DIRECTION::LEFT))
			reloadLevel = true;
	}
	if (m_pPlayer->GetPositionX() > 800 - m_pPlayer->GetSpriteWidth())
	{
		if (m_pLevelManager->GetCurrentLevel(LevelManager::DIRECTION::RIGHT))
			reloadLevel = true;
	}
	if (m_pPlayer->GetPositionY() < 0)
	{
		if (m_pLevelManager->GetCurrentLevel(LevelManager::DIRECTION::UP))
			reloadLevel = true;
	}
	if (m_pPlayer->GetPositionY() > 600 - m_pPlayer->GetSpriteHeight())
	{
		if (m_pLevelManager->GetCurrentLevel(LevelManager::DIRECTION::DOWN))
			reloadLevel = true;
	}

	if (reloadLevel)
	{
		ReloadLevel();
	}

	m_pPlayer->isOnGround = false;
	
	if (m_pPlayer->currentCollisions.size() != 0)
	{
		for (unsigned int i = 0; i < m_pPlayer->currentCollisions.size(); ++i)
		{
			Entity::Collision c;
			c = m_pPlayer->IsCollidingWith(*m_pPlayer->currentCollisions[i]);

			if (c == Entity::Collision::NONE)
			{
				m_pPlayer->currentCollisions.erase(m_pPlayer->currentCollisions.begin() + i);
				m_pPlayer->m_canMoveLeft = true;
				m_pPlayer->m_canMoveRight = true;
				m_pPlayer->isOnGround = false;
			}
			else
			{
				switch (c)
				{
				case Entity::Collision::BOTTOM:
					m_pPlayer->isOnGround = true;
					break;
				case Entity::Collision::LEFT:
					m_pPlayer->m_canMoveRight = false;
					break;
				case Entity::Collision::RIGHT:
					m_pPlayer->m_canMoveLeft = false;
					break;
				}
			}
		}
	}


	// Check for new collisions after processing complete
	for (unsigned int i = 0; i < grass.size(); ++i)
	{
		if (grass[i]->type != Tile::ANCHOR)
		{
			Entity::Collision c;
			c = m_pPlayer->IsCollidingWith(*grass[i]);
			if (c != Entity::Collision::NONE)
			{
				if (m_pPlayer->m_pGrapplingHook->m_RopeAttached)
				{
					MouseReleased();
				}
				// Checks if it is already in container
				if (std::find(m_pPlayer->currentCollisions.begin(), m_pPlayer->currentCollisions.end(), grass[i]) != m_pPlayer->currentCollisions.end()) {
					/* currentCollision<> contains grass */
				}
				else {
					m_pPlayer->currentCollisions.push_back(grass[i]);
				}
				if (grass[i]->type == Tile::DAMAGE)
				{
					m_pLevelManager->PlayerDied();
					ReloadLevel();
					m_pPlayer->SetPositionX(m_pLevelManager->m_xCheckpoint);
					m_pPlayer->SetPositionY(m_pLevelManager->m_yCheckpoint);
					c = Entity::Collision::NONE;
				}
			
			}
			switch (c)
			{
			case  Entity::Collision::NONE:	break;

			case  Entity::Collision::TOP: // Bumps head
					m_pPlayer->m_isJumping = false;
					m_pPlayer->SetPositionY(grass[i]->GetPositionY() + 50);
					m_pPlayer->SetVerticalVelocity(0);
					break;

			case  Entity::Collision::LEFT: // Block to the right
				m_pPlayer->m_canMoveRight = false;
				m_pPlayer->SetPositionX(grass[i]->GetPositionX() - 33);
				break;

			case  Entity::Collision::RIGHT: // Block to the left
				m_pPlayer->m_canMoveLeft = false;
				m_pPlayer->SetPositionX(grass[i]->GetPositionX() + 51);
				break;

			case  Entity::Collision::BOTTOM:
				switch (grass[i]->type)
				{
				case Tile::BOUNCE:
					PlayerJump(1100);
					break;
				default:
					if (!m_pPlayer->m_isJumping)
					{
						m_pPlayer->SetPositionY(grass[i]->GetPositionY() - m_pPlayer->GetSpriteHeight());
						m_pPlayer->SetVerticalVelocity(0);
						m_pPlayer->isOnGround = true;
					};
					break;
				}
				break;
			}
		}
	}
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	backBuffer.DrawSpriteWithTime();

	if (m_pPlayer->m_pGrapplingHook->m_RopeAttached)
	{
		backBuffer.SetDrawColour(0, 255, 0);
		backBuffer.DrawLine((int)m_pPlayer->GetCenterX(), (int)m_pPlayer->GetCenterY(), (int)m_pPlayer->m_pGrapplingHook->m_pAttachedTile->GetCenterX(), (int)m_pPlayer->m_pGrapplingHook->m_pAttachedTile->GetCenterY());
	}
	

	backBuffer.SetDrawColour(0, 0, 0);

	// DRAW LINES:
	/*
	for (int height = 0; height< 600; height+= 50){
		backBuffer.DrawLine(0, height, 800, height);
	}
	for (int width = 0; width< 800; width += 50){
		backBuffer.DrawLine(width, 0, width, 600);
	}*/

	for (unsigned int i = 0; i < grass.size(); ++i)
	{
		grass[i]->Draw(backBuffer);
	}

	// W02.2: Draw all enemy aliens in container...

	// W02.3: Draw all bullets in container...

	// W02.1: Draw the player ship...DONE
	for (unsigned int i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->Draw(backBuffer);
	}
	m_pPlayer->Draw(backBuffer);

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void
Game::MovePlayerLeft()
{
	// W02.1: Tell the player ship to move left. DONE
	if (m_pPlayer->m_canMoveLeft)
	{
		m_pPlayer->SetHorizontalVelocity(-300);
		m_pPlayer->SetAnimationDirection(false);
		m_pPlayer->PauseAnimation(false);
	}
}

// W02.1: Add the method to tell the player ship to move right... DONE
void
Game::MovePlayerRight()
{
	if (m_pPlayer->m_canMoveRight)
	{
		m_pPlayer->SetHorizontalVelocity(300);
		m_pPlayer->SetAnimationDirection(true);
		m_pPlayer->PauseAnimation(false);
	}
}

void
Game::PlayerJump(float height)
{
	if (!m_pPlayer->m_pGrapplingHook->m_RopeAttached)
	{
		if (m_pPlayer->isOnGround)
			m_pPlayer->isOnGround = false;

		m_pPlayer->Jump(height);
	}
}

void
Game::StopMovement()
{
	m_pPlayer->PauseAnimation(true);
	m_pPlayer->SetHorizontalVelocity(0);
}

void
Game::ProcessEnemyMovement(float deltaTime)
{
	// Checks for collisions agains enemies and tiles
	for (unsigned int i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->isOnGround = false;
		for (unsigned int g = 0; g < grass.size(); ++g)
		{
			Entity::Collision c;
			c = enemies[i]->IsCollidingWith(*grass[g]);

			switch (c)
			{
			case  Entity::Collision::NONE:
				break;

			case  Entity::Collision::LEFT: // Block to the right
				enemies[i]->SetPositionX(grass[g]->GetPositionX() - enemies[i]->GetSpriteWidth());
				enemies[i]->moveRight = !enemies[i]->moveRight;
				break;

			case  Entity::Collision::RIGHT: // Block to the left
				enemies[i]->SetPositionX(grass[g]->GetPositionX() + enemies[i]->GetSpriteWidth());
				enemies[i]->moveRight = !enemies[i]->moveRight;
				break;

			case  Entity::Collision::BOTTOM:
				enemies[i]->SetPositionY(grass[g]->GetPositionY() - enemies[i]->GetSpriteHeight());
				enemies[i]->SetVerticalVelocity(0);
				enemies[i]->isOnGround = true;
				break;
			}
		}

		enemies[i]->Process(deltaTime);

		// Checks for collision with player.
		// Enemy 'dies' when player lands on top
		if (enemies[i]->IsCollidingWith(*m_pPlayer) == Entity::Collision::TOP)
		{
			enemies[i]->SetPositionX(550);
			enemies[i]->SetPositionY(200);
			enemies[i]->moveRight = false;
			PlayerJump(400);
		}
		// Kills player for all other collisions
		else if (enemies[i]->IsCollidingWith(*m_pPlayer) != Entity::Collision::NONE)
		{
			m_pLevelManager->PlayerDied();
			ReloadLevel();
			m_pPlayer->SetPositionX(m_pLevelManager->m_xCheckpoint);
			m_pPlayer->SetPositionY(m_pLevelManager->m_yCheckpoint);
		}
	}
}


// W02.3: Space a Bullet in game.
void 
Game::FireSpaceShipBullet()
{
	// W02.3: Load the player bullet sprite.      

	// W02.3: Create a new bullet object.

	// W02.3: Set the bullets vertical velocity.

	// W02.3: Add the new bullet to the bullet container.
}

void
Game::UpdatePlayerPosition(int x, int y)
{
	if (m_pPlayer->GetPositionX() > 700 && m_pPlayer->GetPositionX() < 800)
	{
		m_pPlayer->SetPositionX(0);
	}
	else if (m_pPlayer->GetPositionX() < 100 && m_pPlayer->GetPositionX() > -50 )
	{
		m_pPlayer->SetPositionX(800-m_pPlayer->GetSpriteWidth());
	}
	else if (m_pPlayer->GetPositionY() > 500 && m_pPlayer->GetPositionY() < 600)
	{
		m_pPlayer->SetPositionY(0);
	}
	else if (m_pPlayer->GetPositionY() < 100 && m_pPlayer->GetPositionY() > -50)
	{
		m_pPlayer->SetPositionY(600 - m_pPlayer->GetSpriteHeight());
	}
}


void
Game::SpawnPlayer(int x, int y)
{
	// W02.1: Load the player ship sprite. DONE
	// For example: Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	AnimatedSprite* pPlayerSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\blackwidow.png");
	//Sprite* pPlayerSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\blackwidow1.png");

	// Adds 4 frames to animated sprite
	for (int i = 0; i < pPlayerSprite->GetWidth(); i += pPlayerSprite->GetWidth() / 4)
	{
		pPlayerSprite->AddFrame(i);
	}

	pPlayerSprite->SetFrameSpeed(0.1f);
	pPlayerSprite->SetFrameHeight(pPlayerSprite->GetHeight() / 2);
	pPlayerSprite->SetFrameWidth(pPlayerSprite->GetWidth() / 4);
	pPlayerSprite->SetLooping(true);
	pPlayerSprite->Pause(false);
	pPlayerSprite->StartAnimating();


	// W02.1: Create the player ship instance. DONE
	m_pPlayer = new Player();
	m_pPlayer->InitialiseAnimated(pPlayerSprite);
	m_pPlayer->SetStartPosition(x, y);
	m_pPlayer->SetHeight(pPlayerSprite->GetFrameHeight());
	m_pPlayer->SetWidth(pPlayerSprite->GetFrameWidth());
}
// W02.2: Spawn a Enemy in game.
void 
Game::SpawnEnemy(int x, int y)
{
	AnimatedSprite* enemySprite = m_pBackBuffer->CreateAnimatedSprite("assets\\ghost.png");
	enemySprite->SetFrameSpeed(0.2f);
	enemySprite->SetFrameHeight(enemySprite->GetHeight() / 2);
	enemySprite->SetFrameWidth(enemySprite->GetWidth() / 3);
	enemySprite->SetLooping(true);
	enemySprite->Pause(false);
	enemySprite->StartAnimating();


	Enemy* e = new Enemy();
	e->InitialiseAnimated(enemySprite);
	e->SetPositionX((float)x);
	e->SetPositionY((float)y);
	e->SetHeight(enemySprite->GetFrameHeight());
	e->SetWidth(enemySprite->GetFrameWidth());
	enemies.push_back(e);
}

// TODO: Clean up SPawning methods
void
Game::SpawnTile(int x, int y, Tile::Type type)
{
	Sprite* sprite;
	switch (type)
	{
	case Tile::ANCHOR: 
		sprite = m_pBackBuffer->CreateSprite("assets\\anchor.png");
		break;
	case Tile::GRASS: 
		sprite = m_pBackBuffer->CreateSprite("assets\\grass.png");
		break;
	case Tile::BOUNCE: 
		sprite = m_pBackBuffer->CreateSprite("assets\\bounce.png");
		break;
	case Tile::DAMAGE: 
		sprite = m_pBackBuffer->CreateSprite("assets\\damage.png");
		break;
	case Tile::ENTER: 
		sprite = m_pBackBuffer->CreateSprite("assets\\enter.png");
		break;
	case Tile::EXIT: 
		sprite = m_pBackBuffer->CreateSprite("assets\\exit.png");
		break;
	default: 
		sprite = m_pBackBuffer->CreateSprite("assets\\default.png");
		LogManager::GetInstance().Log("Default texture used! Type switch has not been added!");
		break;
	}

	if (sprite == nullptr)
	{
		LogManager::GetInstance().Log("Failed to load image Game::SpawnTile()");
	}
	else
	{
		Tile* tile = new Tile();
		tile->SetType(type);
		tile->Initialise(sprite);
		tile->SetPositionX((float)x);
		tile->SetPositionY((float)y);
		grass.push_back(tile);
	}
}

void
Game::MouseClick(int x, int y)
{
	for (unsigned int i = 0; i < grass.size(); ++i)
	{
		if (grass[i]->type == Tile::ANCHOR)
		{
			// TODO: Change anchor sprite to one image to save loading two images
			if (x > grass[i]->GetPositionX() && x < grass[i]->GetPositionX() + grass[i]->GetSpriteWidth() &&
				y > grass[i]->GetPositionY() && y < grass[i]->GetPositionY() + grass[i]->GetSpriteHeight())
			{
				m_pPlayer->m_pGrapplingHook->Attach(grass[i]);
				m_pPlayer->m_pGrapplingHook->SetSwingDirection(m_pPlayer->GetPositionX());
				Sprite* s = m_pBackBuffer->CreateSprite("assets\\anchorclicked.png");
				grass[i]->ChangeSprite(s);
			}
		}
	}
}

void
Game::MouseReleased()
{
	for (unsigned int i = 0; i < grass.size(); ++i)
	{
		if (grass[i]->type == Tile::ANCHOR)
		{
			if (grass[i]->clicked)
			{
				m_pPlayer->m_pGrapplingHook->Detach();
				m_pPlayer->SetHorizontalVelocity(0);
				m_pPlayer->SetVerticalVelocity(0);
				PlayerJump(800);
				Sprite* s = m_pBackBuffer->CreateSprite("assets\\anchor.png");
				grass[i]->ChangeSprite(s);
			}
		}
	}
}

void
Game::ReloadLevel()
{
	for (unsigned int i = 0; i < grass.size(); ++i)
	{
		delete grass[i];
	}
	grass.clear();

	for (unsigned int i = 0; i < enemies.size(); ++i)
	{
		delete enemies[i];
	}
	enemies.clear();
	m_pPlayer->currentCollisions.clear();
	m_pLevelManager->CreateNewLevel(*this);
}

void
Game::SetGameState(GameState toSet)
{
	m_currentGameState = toSet;
}

void
Game::UpdateMenu(int x, int y)
{
	m_pMenu->Click(x, y);
}


