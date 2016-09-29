#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "animatedsprite.h"
#include "grapplinghook.h"

#include <vector>

class Player : public Entity
{
public:
	Player();
	~Player();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);
	void Jump(float height);
	
	void IsOnGround(bool value);

	void SetStartPosition(int x, int y);

	

	inline int GetSpriteWidth() { return m_pAnimatedSprite->GetFrameWidth(); }
	inline int GetSpriteHeight() { return m_pAnimatedSprite->GetFrameHeight(); }

	// Member Data

	std::vector<Entity*> currentCollisions;
	bool isOnGround;
	const int gravity;

	GrapplingHook* m_pGrapplingHook;
	bool m_hookAttached;

	float m_startingX;
	float m_startingY;
	
};

#endif // __PLAYER_H__