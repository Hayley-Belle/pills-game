#ifndef __TILE_H__
#define __TILE_H__

#include "entity.h"


class Tile : public Entity
{
public:
	Tile();
	~Tile();

	enum Type
	{
		GRASS,
		DAMAGE,
		BOUNCE,
		ENTER,
		EXIT,
		ANCHOR
	};

	void SetType(Type t);
	void ChangeSprite(Sprite* s);

public:
	Type type;
	bool clicked;

	

};

#endif // __TILE_H__