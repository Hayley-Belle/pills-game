#include "player.h"
#include "sprite.h"
#include "animatedsprite.h"

Player::Player() :
gravity(42)
, isOnGround(0)
, currentCollisions(0)
, m_pGrapplingHook(0)
, m_hookAttached(0)
{
	m_pGrapplingHook = new GrapplingHook();
	m_canMoveLeft = true;
	m_canMoveRight = true;
}

Player::~Player()
{
	delete m_pGrapplingHook;
	m_pGrapplingHook = 0;

	// DOES NOT NEED TO DELETE CURRENT COLLISIONS GAME::~GAME() DOES THIS
}

void
Player::Process(float deltaTime)
{

	if (m_pAnimatedSprite != nullptr)
	{
		m_pAnimatedSprite->SetX(static_cast<int>(m_x));
		m_pAnimatedSprite->SetY(static_cast<int>(m_y));
		m_pAnimatedSprite->Process(deltaTime);
	}

	if (m_pGrapplingHook->m_RopeAttached)
	{
		m_pGrapplingHook->SetPositionX(m_x);
		m_pGrapplingHook->SetPositionY(m_y);
		m_pGrapplingHook->Swing(deltaTime);
		m_x = m_pGrapplingHook->GetPositionX();
		m_y = m_pGrapplingHook->GetPositionY();
	}
	else
	{
		// Processes gravity
		if (isOnGround)
		{
			m_velocityY = 0;
		}
		else{
			m_velocityY += gravity;
			if (m_velocityY <= 0) /* Once player has reached the peak of the jump */
			{
				m_isJumping = false;
			}
		}


		// Stops player falling throught the bottom of the screen. Might disable this and replace with death notice. 
		/*if (m_y > 600 - m_pSprite->GetHeight())
		{
			m_y = 600 - m_pSprite->GetHeight();
			isOnGround = true;
		}*/
		Entity::Process(deltaTime);
	}
}

void
Player::Draw(BackBuffer& backbuffer)
{
	m_pAnimatedSprite->Draw(backbuffer);
}

void
Player::Jump(float height)
{
	if (!m_isJumping)
	{
		m_velocityY = -height;
		m_isJumping = true;
	}
	// TODO: MAX HEIGHT OF JUMP = 2.5 UNIT ~ 125px;
	// TODO: MAX DISTANCE = 4 UNIT ~ 200 px;
	// TODO: NEED TO WORK ON JUMP TIMING -- Possibly 0.44seconds to reach 2.5 units high
}

void
Player::IsOnGround(bool value)
{
	isOnGround = value;
}

void
Player::SetStartPosition(int x, int y)
{
	m_startingX = x;
	m_startingY = y;

	m_x = m_startingX;
	m_y = m_startingY;
}

