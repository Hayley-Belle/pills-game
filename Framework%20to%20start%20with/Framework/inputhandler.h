// COMP710 GP 2D Framework
#ifndef __INPUTHANDER_H__
#define __INPUTHANDER_H__

// Library includes:
#include <SDL.h>
#include <map>

// Forward Declarations:
class Game;
class Menu;

class InputHandler
{
	//Member Methods:
public:
	InputHandler();
	~InputHandler();

	bool Initialise();
	void ProcessInput(Game& game);
	void Process(Game& game);
	void ProcessMouse(Menu& menu);

protected:

private:
	InputHandler(const InputHandler& inputHandler);
	InputHandler& operator=(const InputHandler& inputHandler);

	//Member Data:
public:
	typedef enum{
		LEFT,
		RIGHT,
		JUMP,
	} Direction;

protected:
	SDL_Joystick* m_pGameController;
	std::map<Direction, bool> m_pressedKeys;

private:

};

#endif // __INPUTHANDER_H__
