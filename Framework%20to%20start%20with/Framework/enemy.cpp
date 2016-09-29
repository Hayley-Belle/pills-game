#include "enemy.h"
#include "sprite.h"
#include "backbuffer.h"
#include "animatedsprite.h"

Enemy::Enemy()
	: moveRight(0)
	, isOnGround(0)
{

}

Enemy::~Enemy()
{

}

void
Enemy::Process(float deltaTime)
{
	if (m_pAnimatedSprite != nullptr)
	{
		m_pAnimatedSprite->SetX(static_cast<int>(m_x));
		m_pAnimatedSprite->SetY(static_cast<int>(m_y));
		m_pAnimatedSprite->Process(deltaTime);
	}

	if (moveRight)
	{
		m_velocityX = 150;
		SetAnimationDirection(true);
	}
	else
	{
		m_velocityX = -150;
		SetAnimationDirection(false);
	}

	if (m_y > 600 - height)
	{
		m_y = 600 - height;
		isOnGround = true;
	}

	// Processes gravity
	if (isOnGround)
	{
		m_velocityY = 0;
	}
	else{
		m_velocityY += 42;
		m_y += m_velocityY * deltaTime;
	}

	m_x += m_velocityX * deltaTime;
	

	if (m_x > 800 - width)
	{
		m_x = 800 - width;
		moveRight = !moveRight;
	}

	if (m_x < 0)
	{
		m_x = 0;
		moveRight = !moveRight;
	}
}

void
Enemy::Draw(BackBuffer& backbuffer)
{
	m_pAnimatedSprite->Draw(backbuffer);
}