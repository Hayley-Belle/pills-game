// COMP710 GP 2D Framework
#ifndef __ENTITY_H__
#define __ENTITY_H__

// Forward Declarations:
class BackBuffer;
class Sprite;
class AnimatedSprite;

class Entity
{
public:
	typedef enum
	{
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		NONE
	} Collision;

	//Member Methods:
public:
	Entity();
	~Entity();

	bool Initialise(Sprite* sprite);
	bool InitialiseAnimated(AnimatedSprite* sprite);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetDead(bool dead);
	bool IsDead() const;

	Collision IsCollidingWith(Entity& e);

	float GetPositionX();
	float GetPositionY();

	float GetCenterX();
	float GetCenterY();

	void SetPositionX(float x);
	void SetPositionY(float y);

	float GetHorizontalVelocity();
	void SetHorizontalVelocity(float x); 

	float GetVerticalVelocity();
	void SetVerticalVelocity(float y);

	float GetSpriteWidth();
	float GetSpriteHeight();

	void SetHeight(int size);
	void SetWidth(int size);

	void SetAnimationDirection(bool b);
	void PauseAnimation(bool b);

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	//Member Data:
public:

protected:
	Sprite* m_pSprite;
	AnimatedSprite* m_pAnimatedSprite;

	float m_x;
	float m_y;

	float m_velocityX;
	float m_velocityY;

	int width;
	int height;

	bool m_dead;

public:
	bool m_canMoveRight,
		m_canMoveLeft,
		m_isJumping;

private:

};

#endif //__ENTITY_H__
