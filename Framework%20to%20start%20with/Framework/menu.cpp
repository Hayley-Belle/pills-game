#include "menu.h"
// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "game.h"
#include "logmanager.h"
// Library includes:
#include <cassert>
Menu::Menu()
	: m_SCREEN_WIDTH(800)
	, m_SCREEN_HEIGHT(600)
	, m_buttonCenterX(400)
	, m_bPlay(200)
	, m_bInstructions(300)
	, m_bCredits(400)
	, m_bQuit(500)
	, m_MenuBackground(0)
	, m_Button_Instructions(0)
	, m_Button_Credits(0)
	, m_Button_Quit(0)
	, m_Button_Back(0)
	, m_Button_Resume(0)
	, m_Button_MainMenu(0)
	, m_title_Lose(0)
	, m_title_Win(0)
	, m_buttonWidth(0)
	, m_buttonHeight(0)
{
}
Menu::~Menu()
{
	delete m_MenuBackground;
	m_MenuBackground = 0;

	delete m_Button_Play;
	m_Button_Play = 0;

	delete m_Button_Instructions;
	m_Button_Instructions = 0;
	
	delete m_Button_Credits;
	m_Button_Credits = 0;
	
	delete m_Button_Quit;
	m_Button_Quit = 0;

	delete m_Button_Back;
	m_Button_Back = 0;
	
	delete m_Button_Resume;
	m_Button_Resume = 0;
	
	delete m_Button_MainMenu;
	m_Button_MainMenu = 0;
	
	delete m_title_Lose;
	m_title_Lose = 0;
	
	delete m_title_Win;
	m_title_Win = 0;
}
bool
Menu::Initialise(BackBuffer* backBuffer)
{
	m_MenuBackground = backBuffer->CreateSprite("assets\\menu\\menubackground.png");
	m_Button_Play = backBuffer->CreateSprite("assets\\menu\\button_play.png");
	m_Button_Instructions = backBuffer->CreateSprite("assets\\menu\\button_instructions.png");
	m_Button_Credits = backBuffer->CreateSprite("assets\\menu\\button_credits.png");
	m_Button_Quit = backBuffer->CreateSprite("assets\\menu\\button_quit.png");
	m_Button_Back = backBuffer->CreateSprite("assets\\menu\\button_back.png");
	m_Button_Resume = backBuffer->CreateSprite("assets\\menu\\button_resume.png");
	m_Button_MainMenu = backBuffer->CreateSprite("assets\\menu\\button_mainmenu.png");

	m_buttonWidth = m_Button_Play->GetWidth();
	m_buttonHeight = m_Button_Play->GetHeight();

	m_title_Win = backBuffer->CreateSprite("assets\\menu\\win.png");
	//m_title_Lose = backBuffer->CreateSprite("assets\\menu\\title_lose.png");*/
	return (true);
}
void
Menu::Draw(BackBuffer* backBuffer)
{
	backBuffer->Clear();
	assert(m_MenuBackground);
	m_MenuBackground->Draw(*backBuffer);
	switch (Game::GetInstance().GetGameState())
	{
	case Game::GameState::TITLE:

		assert(m_Button_Play);
		m_Button_Play->SetX(m_buttonCenterX);
		m_Button_Play->SetY(m_bPlay);
		m_Button_Play->Draw(*backBuffer);
		assert(m_Button_Instructions);
		m_Button_Instructions->SetX(m_buttonCenterX);
		m_Button_Instructions->SetY(m_bInstructions);
		m_Button_Instructions->Draw(*backBuffer);
		assert(m_Button_Credits);
		m_Button_Credits->SetX(m_buttonCenterX);
		m_Button_Credits->SetY(m_bCredits);
		m_Button_Credits->Draw(*backBuffer);
		assert(m_Button_Quit);
		m_Button_Quit->SetX(m_buttonCenterX);
		m_Button_Quit->SetY(m_bQuit);
		m_Button_Quit->Draw(*backBuffer);
		break;

	case Game::GameState::INSTRUCTIONS:

		assert(m_Button_Back);
		m_Button_Back->SetX(600);
		m_Button_Back->SetY(m_bQuit);
		m_Button_Back->Draw(*backBuffer);
		break;

	case Game::GameState::CREDITS:

		assert(m_Button_Back);
		m_Button_Back->SetX(600);
		m_Button_Back->SetY(m_bQuit);
		m_Button_Back->Draw(*backBuffer);
		break;

	case Game::GameState::PAUSE:

		assert(m_Button_Resume);
		m_Button_Resume->SetX(m_buttonCenterX);
		m_Button_Resume->SetY(m_bPlay);
		m_Button_Resume->Draw(*backBuffer);
		assert(m_Button_Instructions);
		m_Button_Instructions->SetX(m_buttonCenterX);
		m_Button_Instructions->SetY(m_bInstructions);
		m_Button_Instructions->Draw(*backBuffer);
		assert(m_Button_MainMenu);
		m_Button_MainMenu->SetX(m_buttonCenterX);
		m_Button_MainMenu->SetY(m_bQuit);
		m_Button_MainMenu->Draw(*backBuffer);
		break;

	case Game::GameState::WIN:

		assert(m_title_Win);
		m_title_Win->SetX(0);
		m_title_Win->SetY(0);
		m_title_Win->Draw(*backBuffer);
		assert(m_Button_MainMenu);
		m_Button_MainMenu->SetX(m_buttonCenterX);
		m_Button_MainMenu->SetY(m_bCredits);
		m_Button_MainMenu->Draw(*backBuffer);
		assert(m_Button_Quit);
		m_Button_Quit->SetX(m_buttonCenterX);
		m_Button_Quit->SetY(m_bQuit);
		m_Button_Quit->Draw(*backBuffer);
		break;

	//case Game::GameState::LOSE:

	//	assert(m_title_Lose);
	//	m_title_Lose->SetX(m_buttonCenterX);
	//	m_title_Lose->SetY(m_bPlay);
	//	m_title_Lose->Draw(*backBuffer);
	//	assert(m_Button_MainMenu);
	//	m_Button_MainMenu->SetX(m_buttonCenterX);
	//	m_Button_MainMenu->SetY(m_bCredits);
	//	m_Button_MainMenu->Draw(*backBuffer);
	//	assert(m_Button_Quit);
	//	m_Button_Quit->SetX(m_buttonCenterX);
	//	m_Button_Quit->SetY(m_bQuit);
	//	m_Button_Quit->Draw(*backBuffer);
	//	break;

	default: // TODO: REMOVE THIS
		break;

	}
	backBuffer->Present();
}
void
Menu::Click(int x, int y)
{
	LogManager::GetInstance().Log("MouseClicked!");
	switch (Game::GetInstance().GetGameState())
	{
	case Game::GameState::TITLE:
		if (x > m_Button_Play->GetX() && x < m_Button_Play->GetX() + m_buttonWidth &&
			y > m_Button_Play->GetY() && y < m_Button_Play->GetY() + m_buttonHeight)
		{
			Game::GetInstance().SetGameState(Game::GameState::PLAY);
		}
		else if (x > m_Button_Instructions->GetX() && x < m_Button_Instructions->GetX() + m_buttonWidth &&
			y > m_Button_Instructions->GetY() && y < m_Button_Instructions->GetY() + m_buttonHeight)
		{
			Game::GetInstance().SetGameState(Game::GameState::INSTRUCTIONS);
		}
		else if (x > m_Button_Credits->GetX() && x < m_Button_Credits->GetX() + m_buttonWidth &&
			y > m_Button_Credits->GetY() && y < m_Button_Credits->GetY() + m_buttonHeight)
		{
			Game::GetInstance().SetGameState(Game::GameState::CREDITS);
		}
		else if (x > m_Button_Quit->GetX() && x < m_Button_Quit->GetX() + m_buttonWidth &&
			y > m_Button_Quit->GetY() && y < m_Button_Quit->GetY() + m_buttonHeight)
		{
			Game::GetInstance().SetGameState(Game::GameState::EXIT);
		}
		break;
	case Game::GameState::INSTRUCTIONS:
		if (x > 600 && x < 600 + m_buttonWidth)
		{
			if (y > m_bQuit && y < m_bQuit + m_buttonHeight)
				Game::GetInstance().SetGameState(Game::GameState::TITLE);
		}
		break;
	case Game::GameState::CREDITS:
		if (x > 600 && x < 600 + m_buttonWidth)
		{
			if (y > m_bQuit && y < m_bQuit + m_buttonHeight)
				Game::GetInstance().SetGameState(Game::GameState::TITLE);
		}
		break;
	case Game::GameState::PAUSE:
		if (x > m_Button_Resume->GetX() && x < m_Button_Resume->GetX() + m_buttonWidth &&
			y > m_Button_Resume->GetY() && y < m_Button_Resume->GetY() + m_buttonHeight)
		{
			Game::GetInstance().SetGameState(Game::GameState::PLAY);
		}
		else if (x > m_Button_MainMenu->GetX() && x < m_Button_MainMenu->GetX() + m_buttonWidth &&
			y > m_Button_MainMenu->GetY() && y < m_Button_MainMenu->GetY() + m_buttonHeight)
		{
			Game::GetInstance().SetGameState(Game::GameState::TITLE);
		}
		break;
	case Game::GameState::WIN:
		if (x > m_buttonCenterX && x < m_buttonCenterX + m_buttonWidth)
		{
			if (y > m_bCredits && y < m_bCredits + m_buttonHeight)
				Game::GetInstance().SetGameState(Game::GameState::TITLE);
		}
		if (x > m_Button_Quit->GetX() && x < m_Button_Quit->GetX() + m_buttonWidth &&
			y > m_Button_Quit->GetY() && y < m_Button_Quit->GetY() + m_buttonHeight)
		{
			Game::GetInstance().SetGameState(Game::GameState::EXIT);
		}
		break;
	case Game::GameState::LOSE:
		if (x > m_buttonCenterX && x < m_buttonCenterX + m_buttonWidth)
		{
			if (y > m_bCredits && y < m_bCredits + m_buttonHeight)
				Game::GetInstance().SetGameState(Game::GameState::TITLE);
		}
		if (x > m_Button_Quit->GetX() && x < m_Button_Quit->GetX() + m_buttonWidth &&
			y > m_Button_Quit->GetY() && y < m_Button_Quit->GetY() + m_buttonHeight)
		{
			Game::GetInstance().SetGameState(Game::GameState::EXIT);
		}
		break;
	default:
		break;
	}
}