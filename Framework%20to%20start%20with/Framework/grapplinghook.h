#ifndef __GRAPPLINGHOOK_H__
#define __GRAPPLINGHOOK_H__

#include "entity.h"
#include "tile.h"

class GrapplingHook : public Entity
{
	// Member functions:
public:
	GrapplingHook();
	~GrapplingHook();

	void Swing(float deltaTime);
	void SetSwingDirection(float playerX);

	void Attach(Tile* tileToAttach);
	void Detach();

	// Member fields:
public:
	bool m_RopeAttached;
	float m_SwingAngle;
	const int MAX_ROPE_LENGTH;
	bool angleSwitch;
	Tile* m_pAttachedTile;
	float m_TileCenterX
		, m_TileCenterY;
	bool m_SwingRight;

	float acceleration;

	float min, max;

};

#endif // __GRAPPLINGHOOK_H__