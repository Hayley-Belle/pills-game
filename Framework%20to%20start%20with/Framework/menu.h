#ifndef __MENU_H__
#define __MENU_H__

#include "entity.h"

// Forward Declarations:
class BackBuffer;
class Sprite;
class Game;

class Menu
{

public:
	Menu();
	~Menu();

	bool Initialise(BackBuffer* backBuffer);
	void Draw(BackBuffer* backBuffer);

	void Click(int x, int y);

	//Screen dimension constants
	const int m_SCREEN_WIDTH;
	const int m_SCREEN_HEIGHT;


protected:
	Sprite* m_MenuBackground;
	Sprite* m_Button_Play;
	Sprite* m_Button_Instructions;
	Sprite* m_Button_Credits;
	Sprite* m_Button_Quit;
	Sprite* m_Button_Back;
	Sprite* m_Button_Resume;
	Sprite* m_Button_MainMenu;
	Sprite* m_title_Win;
	Sprite* m_title_Lose;

	int m_buttonCenterX;

	// Y values for buttons
	int m_bPlay;
	int m_bInstructions;
	int m_bCredits;
	int m_bQuit;

	int m_buttonWidth;
	int m_buttonHeight;

private:
	Menu(const Menu& menu);
	Menu& operator=(const Menu& menu);

};
#endif __MENU_H__
