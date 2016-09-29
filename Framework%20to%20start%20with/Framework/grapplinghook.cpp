// This include:
#include "grapplinghook.h"

// Local includes:
#include "entity.h"
#include "logmanager.h"

// Library includes:
#include <math.h>
#include <cstdio>

GrapplingHook::GrapplingHook()
: m_RopeAttached(0)
, MAX_ROPE_LENGTH(150)
, m_pAttachedTile(0)
, m_SwingAngle(0)
, angleSwitch(0)
, m_SwingRight(0)
, min(10)
, max(0)
, acceleration(0)
{

}

GrapplingHook::~GrapplingHook()
{
	
}

void
GrapplingHook::Swing(float deltaTime)
{
	if (m_RopeAttached)
	{
		float xHold = m_x;
		float yHold = m_y;

		// IF player height > bottom of tile THEN FALL;
		if (m_y <= m_pAttachedTile->GetPositionY() + m_pAttachedTile->GetSpriteHeight())
		{
			m_velocityY += 42;
			m_y += m_velocityY * deltaTime;
		}
		else
		{

			if (m_velocityY > 300)
			{
				//m_velocityY = 300;
				m_velocityY *= 0.7f;
			}
			else if (m_velocityY > 60)
			{
				m_velocityY -= 3;
			}

			float heightDifference = m_y - m_pAttachedTile->GetPositionY() + m_pAttachedTile->GetSpriteHeight();
			float distanceBetween = sqrt((m_x - m_pAttachedTile->GetCenterX()) * (m_x - m_pAttachedTile->GetCenterX())
									+ (m_y - m_pAttachedTile->GetCenterY()) * (m_y - m_pAttachedTile->GetCenterY()));
			float startDistance = distanceBetween;

			
			m_velocityX = heightDifference * 0.7f;

			acceleration = m_velocityX + m_velocityY;
			acceleration *= deltaTime * 0.8;

			if (heightDifference < min)
				min = heightDifference;
			if (heightDifference > max)
				max = heightDifference;

			float angle = (heightDifference / distanceBetween) * acceleration;


			if (m_SwingRight)
			{
				angle *= -1;
			}

			angle *= deltaTime;

			float sinAngle = sin(angle);
			float cosAngle = cos(angle);

		    m_x -= m_TileCenterX;
			m_y -= m_TileCenterY;

			float newX = m_x * cosAngle - m_y * sinAngle;
			float newY = m_x * sinAngle + m_y * cosAngle;


			/////// NEW BELOW/////////////
			float x1 = m_x - m_TileCenterX;
			float y1 = m_y - m_TileCenterY;

			float x2 = x1 * cosAngle - y1 * sinAngle;
			float y2 = x1 * sinAngle + y1 * cosAngle;

			float testm_x = x2 + m_TileCenterX;
			float testm_y = y2 + m_TileCenterY;


			///////////////////////////////////

			// Swing range is 180degres from bottom of attached tile
			if (newY + m_TileCenterY <= m_pAttachedTile->GetPositionY() + m_pAttachedTile->GetSpriteHeight())
			{
				// Switch swing direction and dont calculate new position
				m_SwingRight = !m_SwingRight;
				m_x = xHold;
				m_y = yHold;
			}
			else
			{
				// Calculate new position
				m_x = newX + m_TileCenterX;
				m_y = newY + m_TileCenterY;

				// Update distance between
				distanceBetween = sqrt((m_x - m_pAttachedTile->GetCenterX()) * (m_x - m_pAttachedTile->GetCenterX())
					+ (m_y - m_pAttachedTile->GetCenterY()) * (m_y - m_pAttachedTile->GetCenterY()));

				if (distanceBetween > startDistance)
				{
					float deltaX =  m_pAttachedTile->GetCenterX() - m_x;
					float deltaY =  m_pAttachedTile->GetCenterY() - m_y;
					float theta = atan2(deltaY, deltaX) * 3.14159265358979323846 / 180;
					newX = (distanceBetween - startDistance) * cos(theta);
					newY = (distanceBetween - startDistance) * sin(theta);

					m_x += newX*4;
					m_y += newY*4;

					distanceBetween = sqrt((m_x - m_pAttachedTile->GetCenterX()) * (m_x - m_pAttachedTile->GetCenterX())
						+ (m_y - m_pAttachedTile->GetCenterY()) * (m_y - m_pAttachedTile->GetCenterY()));

					float y = 6 + 12;
				}
			}
		}
	}
}

void
GrapplingHook::Attach(Tile* tileToAttach)
{
	if (tileToAttach != nullptr)
	{
		m_pAttachedTile = tileToAttach;
		m_TileCenterX = m_pAttachedTile->GetPositionX() + 25;
		m_TileCenterY = m_pAttachedTile->GetPositionY() + 25; // m_pAttachedTile->GetSpriteHeight() / 2

		//m_SwingAngle = atan2(m_y - m_TileCenterY, m_x - m_TileCenterX);
		//m_SwingAngle = m_SwingAngle * 180 / 3.141526;

		if (m_pAttachedTile != nullptr && m_TileCenterX != 0 && m_TileCenterY != 0)
		{
			m_RopeAttached = true;
		}
	}
}

void
GrapplingHook::Detach()
{
	m_pAttachedTile = 0;
	m_TileCenterX = 0;
	m_TileCenterY = 0;
	m_SwingAngle = 0;
	m_RopeAttached = false;
}

void
GrapplingHook::SetSwingDirection(float playerX)
{
	// Determine swing direction
	if (playerX < m_pAttachedTile->GetCenterX())
	{
		m_SwingRight = true;
	}
	else
	{
		m_SwingRight = false;
	}
}

