// This include:
#include "animatedsprite.h"

// Library includes:
#include "sprite.h"
#include "backbuffer.h"

AnimatedSprite::AnimatedSprite()
	: m_frameSpeed(0)
	, m_frameWidth(0)
	, m_timeElapsed(0)
	, m_currentFrame(0)
	, m_loop(false)
	, m_paused(false)
	, m_animating(false)
	, m_movingRight(true)
{
	
}

AnimatedSprite::~AnimatedSprite()
{

}

bool
AnimatedSprite::Initialise(Texture& texture)
{
	return Sprite::Initialise(texture);
}

void
AnimatedSprite::AddFrame(int x)
{
	m_pFrameCoordinates.push_back(x);
}

void
AnimatedSprite::Process(float deltaTime)
{
	if (!m_paused)
	{
		m_timeElapsed += deltaTime;
	}
	
	if (m_timeElapsed >= m_frameSpeed)
	{
		// Reset time elapsed for next frame
		m_timeElapsed = 0;

		if (m_loop && m_animating)
		{
			(unsigned int)(m_currentFrame + 1) < m_pFrameCoordinates.size() ? ++m_currentFrame : m_currentFrame = 0;
		}
		else if (!m_loop)
		{
			if ((unsigned int)(m_currentFrame + 1) < m_pFrameCoordinates.size())
			{
				++m_currentFrame;
			}
		}	
	}
}

void
AnimatedSprite::Draw(BackBuffer& backbuffer)
{
	backbuffer.DrawAnimatedSprite(*this);
}

void
AnimatedSprite::SetFrameSpeed(float speed)
{
	m_frameSpeed = speed;
}

void
AnimatedSprite::SetFrameWidth(int width)
{
	m_frameWidth = width;
}

void 
AnimatedSprite::SetFrameHeight(int height)
{
	m_frameHeight = height;
}

void
AnimatedSprite::Pause(bool pause)
{
	if (pause)
	{
		m_currentFrame = 0;
	}
	m_paused = pause;

}

bool
AnimatedSprite::IsPaused()
{
	return m_paused;
}

bool
AnimatedSprite::IsAnimating()
{
	return m_animating;
}

void
AnimatedSprite::StartAnimating()
{
	m_animating = true;
}

bool
AnimatedSprite::IsLooping()
{
	return m_loop;
}

void
AnimatedSprite::SetLooping(bool b)
{
	m_loop = b;
}

void
AnimatedSprite::SetDirection(bool b)
{
	m_movingRight = b;
}
