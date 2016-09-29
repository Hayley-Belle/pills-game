#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

public:
	bool moveRight;
	bool isOnGround;
};

#endif // __ENEMY_H__