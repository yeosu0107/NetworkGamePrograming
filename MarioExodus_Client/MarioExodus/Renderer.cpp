#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
	if(m_ppTexture)
		delete[](m_ppTexture);
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	m_Initialized = true;

	m_ppTexture = new Texture*[12];
	m_ppTexture[Texture::TextureNumber::Mario_Sprite]	= new Texture("Resource\\Texture\\Mario_52_52.bmp", 4, 4);
	m_ppTexture[Texture::TextureNumber::Mario_None]		= new Texture("Resource\\Texture\\Mario_Null_52_52.bmp", 1, 1);
	m_ppTexture[Texture::TextureNumber::Marios_Number]	= new Texture("Resource\\Texture\\Number_120_18.bmp", 6 ,1);
	m_ppTexture[Texture::TextureNumber::BackSky]		= new Texture("Resource\\Texture\\BackGround_1280_720.bmp", 1 ,1);
	m_ppTexture[Texture::TextureNumber::Block]			= new Texture("Resource\\Texture\\Block_52_52.bmp", 1 , 1);
	m_ppTexture[Texture::TextureNumber::Door]			= new Texture("Resource\\Texture\\Door_52_52.bmp", 2 , 1);
	m_ppTexture[Texture::TextureNumber::Key]			= new Texture("Resource\\Texture\\Key_30_30.bmp", 1 ,1 );
	m_ppTexture[Texture::TextureNumber::Key_Dish]		= new Texture("Resource\\Texture\\Key_Dish_37_9.bmp", 1 , 1);
	m_ppTexture[Texture::TextureNumber::Main_Rogo]		= new Texture("Resource\\Texture\\Main_Rogo_528_168.bmp", 1 ,1 );
	m_ppTexture[Texture::TextureNumber::Press_C]		= new Texture("Resource\\Texture\\Press_C_Start_326_78.bmp", 1 , 1);
	m_ppTexture[Texture::TextureNumber::Wall]			= new Texture("Resource\\Texture\\Wall_52_52.bmp", 1, 1);
	m_ppTexture[Texture::TextureNumber::GameOver]		= new Texture("Resource\\Texture\\GameOver_528_300.bmp", 1, 1);
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::DrawSolidRect(Vector2& vecPos, int size)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	
		glVertex2d(vecPos.x - size / 2, vecPos.y - size / 2);
		glVertex2d(vecPos.x + size / 2, vecPos.y - size / 2);
		glVertex2d(vecPos.x + size / 2, vecPos.y + size / 2);
		glVertex2d(vecPos.x - size / 2, vecPos.y + size / 2);

	glEnd();
}

void Renderer::DrawSolidRect(Vector2& vec2Pos, Vector2& vec2Size, Vector2& spriteCount, Texture::TextureNumber eTextureNumber)
{
	m_ppTexture[eTextureNumber]->SetTexture();
	Vector2 vec2SpriteCount = m_ppTexture[eTextureNumber]->GetSpriteMaxCount();

	float fXStartPos = vec2SpriteCount.x == 1 ? 0.0f : spriteCount.x / (float)vec2SpriteCount.x;
	float fXEndPos =	(float)(spriteCount.x + 1) / (float)vec2SpriteCount.x;

	float fYStartPos = vec2SpriteCount.y == 1 ? 0.0f : spriteCount.y / (float)vec2SpriteCount.y;
	float fYEndPos = (float)(spriteCount.y + 1) / (float)vec2SpriteCount.y;

	glBegin(GL_QUADS);

		glTexCoord2f(fXStartPos, fYStartPos);
		glVertex2d(vec2Pos.x - vec2Size.x / 2, vec2Pos.y - vec2Size.y / 2);

		glTexCoord2f(fXStartPos, fYEndPos);
		glVertex2d(vec2Pos.x - vec2Size.x / 2, vec2Pos.y + vec2Size.y / 2);

		glTexCoord2f(fXEndPos, fYEndPos);
		glVertex2d(vec2Pos.x + vec2Size.x / 2, vec2Pos.y + vec2Size.y / 2);

		glTexCoord2f(fXEndPos, fYStartPos);
		glVertex2d(vec2Pos.x + vec2Size.x / 2, vec2Pos.y - vec2Size.y / 2);

	glEnd();

	glDisable(GL_TEXTURE_2D);

}