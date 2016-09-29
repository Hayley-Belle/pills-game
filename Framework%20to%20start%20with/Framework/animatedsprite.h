#ifndef __ANIMATEDSPRITE_H__
#define __ANIMATEDSPRITE_H__

#include "sprite.h"

// Library includes:
#include <vector>

class BackBuffer;
class Texture;

class AnimatedSprite : public Sprite
{
	// Methods:
public:
	AnimatedSprite();
	~AnimatedSprite();

	bool Initialise(Texture& texture);
	void AddFrame(int x);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SetFrameSpeed(float f);
	void SetFrameWidth(int w);
	void SetFrameHeight(int h);

	void Pause(bool b);
	bool IsPaused();

	bool IsAnimating();
	void StartAnimating();

	bool IsLooping();
	void SetLooping(bool b);

	void SetDirection(bool b);

	inline int GetDirection() { return m_movingRight; }
	inline int GetCurrentFrame() { return m_currentFrame; }
	inline int GetFrameWidth() { return m_frameWidth; }
	inline int GetFrameHeight() { return m_frameHeight; }
	

protected:

private:

	// Fields:
public:

protected:
	// W02.4: Add a field, the container for frame coordinates.
	std::vector<int> m_pFrameCoordinates;

	float m_frameSpeed;
	int m_frameWidth;
	int m_frameHeight;

	float m_timeElapsed;
	int m_currentFrame;

	bool m_paused;
	bool m_loop;
	bool m_animating;

	bool m_movingRight;

private:

};

#endif // __ANIMATEDSPRITE_H__