// COMP710 GP 2D Framework

// This include:
#include "backbuffer.h"

// Local includes:
#include "logmanager.h"
#include "texturemanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "texture.h"

// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <cassert>

BackBuffer::BackBuffer()
: m_pTextureManager(0)
, m_pWindow(0)
, m_pRenderer(0)
, m_width(0)
, m_height(0)
, m_clearRed(0xFF)
, m_clearGreen(0xFF)
, m_clearBlue(0xFF)
, background(0)
, BackgroundCounter(0)
{
	SetClearColour(37, 37, 38);
}

BackBuffer::~BackBuffer()
{
	delete m_pTextureManager;
	m_pTextureManager = 0;

	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = 0;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = 0;

	delete background;
	background = 0;

	IMG_Quit();
	SDL_Quit();
}

bool 
BackBuffer::Initialise(int width, int height)
{
	m_width = width;
	m_height = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		LogSDLError();
		return (false);
	}
	else
	{
		m_pWindow = SDL_CreateWindow("717130 Game Framework", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

		if (m_pWindow == 0)
		{
			LogSDLError();
			return (false);
		}
		else
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED); 

			if (m_pRenderer == 0)
			{
				LogSDLError();
				return (false);
			}
			else
			{
				int imageResult = IMG_Init(IMG_INIT_PNG);

				if (imageResult == 0)
				{
					LogSDLError();
					return (false);
				}
			}
		}
	}

	m_pTextureManager = new TextureManager();
	assert(m_pTextureManager);
	m_pTextureManager->Initialise(m_pRenderer);

	return (true);
}

void 
BackBuffer::Clear()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_clearRed, m_clearGreen, m_clearBlue, 0xff);
	SDL_RenderClear(m_pRenderer);
}

void
BackBuffer::Present()
{
	SDL_RenderPresent(m_pRenderer);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColour(r, g, b, 0xFF);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
}

void 
BackBuffer::DrawSprite(Sprite& sprite)
{
	SDL_Rect dest;

	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = sprite.GetWidth();
	dest.h = sprite.GetHeight();

	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), 0, &dest);
}

void
BackBuffer::DrawAnimatedSprite(AnimatedSprite& sprite)
{
	// TODO:
	SDL_Rect dest; // SIZE AND POSITION TO DISPLAY INSIDE WINDOW

	dest.x = sprite.GetX();
	dest.y = sprite.GetY() + 3;
	dest.w = sprite.GetFrameWidth();
	dest.h = sprite.GetFrameHeight();

	SDL_Rect srce; // SIZE OF TEXTURE SEGMENT

	srce.x = sprite.GetCurrentFrame()*sprite.GetFrameWidth();
	sprite.GetDirection() ? srce.y = sprite.GetFrameHeight() : srce.y = 0;
	srce.w = sprite.GetFrameWidth();
	srce.h = sprite.GetFrameHeight();
	
	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), &srce, &dest);
}

void
BackBuffer::Process(float deltaTime)
{
	BackgroundCounter += deltaTime;
}

void
BackBuffer::DrawSpriteWithTime()
{
	if (background == nullptr){
		background = CreateSprite("assets\\background.png");
		background->SetX(0);
		background->SetY(0);
	}
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = 800; // SIZE AND POSITION TO DISPLAY INSIDE WINDOW
	dest.h = 600;

	SDL_Rect srce;
	srce.x = BackgroundCounter;
	srce.y = 0;
	srce.w = 800;	// SIZE OF TEXTURE SEGMENT
	srce.h = 600;

	//BackgroundCounter += 0.35f;

	if (BackgroundCounter >= 4000)
	{
		BackgroundCounter = 0;
	}
	if (BackgroundCounter >= 1200 && BackgroundCounter <= 2000)
		srce.x = 1200;
	if (BackgroundCounter >= 2000)
	{
		srce.x = 1200 - (BackgroundCounter - 2000);
		if (BackgroundCounter >= 3200 && BackgroundCounter <= 6000)
			srce.x = 0;
	}

	SDL_RenderCopy(m_pRenderer, background->GetTexture()->GetTexture(), &srce, &dest);
}

void
BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2)
{
	SDL_Rect fillRect;
	
	fillRect.x = x1;
	fillRect.y = y1;
	fillRect.w = x2 - x1;
	fillRect.h = y2 - y1;

	SDL_RenderFillRect(m_pRenderer, &fillRect);
}

void
BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void 
BackBuffer::LogSDLError()
{
	LogManager::GetInstance().Log(SDL_GetError());
}

Sprite* 
BackBuffer::CreateSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	Sprite* pSprite = new Sprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

AnimatedSprite*
BackBuffer::CreateAnimatedSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	AnimatedSprite* pSprite = new AnimatedSprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Animated Sprite Failed to Create!");
	}

	return (pSprite);
}

void 
BackBuffer::SetClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_clearRed = r;
	m_clearGreen = g;
	m_clearBlue = b;
}
