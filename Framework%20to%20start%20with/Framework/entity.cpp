// COMP710 GP 2D Framework

// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "animatedsprite.h"

// Library includes:
#include <cassert>
#include <cmath>

Entity::Entity()
: m_pSprite(0)
, m_pAnimatedSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(false)
, m_canMoveLeft(true)
, m_canMoveRight(true)
, m_isJumping(false)
{

}

Entity::~Entity()
{
	delete m_pSprite;
	m_pSprite = 0;

	delete m_pAnimatedSprite;
	m_pAnimatedSprite = 0;
}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;
	m_pSprite->SetX((int)m_x);
	m_pSprite->SetY((int)m_y);

	height = sprite->GetHeight();
	width = sprite->GetWidth();

	return (true);
}

bool
Entity::InitialiseAnimated(AnimatedSprite* sprite)
{
	assert(sprite);
	m_pAnimatedSprite = sprite;
	m_pAnimatedSprite->SetX((int)m_x);
	m_pAnimatedSprite->SetY((int)m_y);


	return (true);
}

void 
Entity::Process(float deltaTime)
{
	if (m_pSprite != nullptr)
	{
		m_pSprite->SetX(static_cast<int>(m_x));
		m_pSprite->SetY(static_cast<int>(m_y));
	}

	// W02.1: Generic position update, based upon velocity (and time). DONE
	if (m_canMoveLeft && m_velocityX < 0)
	{
		m_x += m_velocityX * deltaTime;
	}
	else if (m_canMoveRight && m_velocityX > 0)
	{
		m_x += m_velocityX * deltaTime;
	}
	else
	{
		m_velocityX = 0;
	}
	
	if (!m_isJumping)
	m_y += m_velocityY * deltaTime;
	
	// W02.1: Boundary checking and position capping. DONE
}

void 
Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->Draw(backBuffer);

}

Entity::Collision
Entity::IsCollidingWith(Entity& e)
{
	//  Minkowski sum
	float w = 0.5 * (width + e.width); // Width of both
	float h = 0.5 * (height + e.height); // Height of both
	float dx = (m_x + width*0.5) - (e.m_x + e.width*0.5); // Center A.x - Center B.x
	float dy = (m_y + height*0.5) - (e.m_y + e.height*0.5); // Center A.y - Center B.y

	if (abs(dx) <= w && abs(dy) <= h)
	{
		/* collision! */
		float wy = w * dy;
		float hx = h * dx;

		if (wy > hx)
		{
			if (wy > -hx)
			{
				/* collision at the top */
				return TOP;
			}
			else
			{
				/* on the left */
				return LEFT;
			}
		}
		else
		{
			if (wy > -hx)
			{
				/* on the right */
				return RIGHT;
			}
			else
			{
				/* at the bottom */
				return BOTTOM;
			}
		}
	}

	// W02.3: Generic Entity Collision routine.	

	return NONE; // W02.4 Change return value!
}

void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

float 
Entity::GetPositionX()
{
	return (m_x);
}

float 
Entity::GetPositionY()
{
	return (m_y);
}

float 
Entity::GetHorizontalVelocity()
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity()
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}

float
Entity::GetSpriteHeight()
{
	return height;
}

float
Entity::GetSpriteWidth()
{
	return width;
}

float
Entity::GetCenterX()
{
	return m_x + width / 2;
}

float
Entity::GetCenterY()
{
	return m_y + height / 2;
}

void
Entity::SetHeight(int size)
{
	height = size;
}

void
Entity::SetWidth(int size)
{
	width = size;
}

void
Entity::SetAnimationDirection(bool b)
{
	m_pAnimatedSprite->SetDirection(b);
}

void
Entity::PauseAnimation(bool b)
{
	m_pAnimatedSprite->Pause(b);
}