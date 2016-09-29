#include "tile.h"
#include "backbuffer.h"
#include "entity.h"

Tile::Tile()
: clicked(0)
{

}

Tile::~Tile()
{

}

void
Tile::SetType(Type t)
{
	type = t;
}

void
Tile::ChangeSprite(Sprite* sprite)
{
	delete m_pSprite;
	m_pSprite = sprite;
	clicked = !clicked;
	Initialise(m_pSprite);
	Process(0);
}
