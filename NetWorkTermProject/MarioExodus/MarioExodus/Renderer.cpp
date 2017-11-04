#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
	delete[](m_pTexture);
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	m_Initialized = true;

	m_pTexture = new Texture*[10];
	m_pTexture[Texture::TextureNumber::Mario_Sprite]	= new Texture("Resource\\Mario_52_52.bmp", 4, 2);
	m_pTexture[Texture::TextureNumber::Mario_None]		= new Texture("Resource\\Mario_Null_52_52.bmp", 1, 1);
	m_pTexture[Texture::TextureNumber::Marios_Number]	= new Texture("Resource\\Number_120_18.bmp", 6 ,1);
	m_pTexture[Texture::TextureNumber::BackSky]			= new Texture("Resource\\BackGround_962_700.bmp", 1 ,1);
	m_pTexture[Texture::TextureNumber::Block]			= new Texture("Resource\\Block_48_48.bmp", 1 , 1);
	m_pTexture[Texture::TextureNumber::Door]			= new Texture("Resource\\Door_48_48.bmp", 2 , 1);
	m_pTexture[Texture::TextureNumber::Key]				= new Texture("Resource\\Key_30_30.bmp", 1 ,1 );
	m_pTexture[Texture::TextureNumber::Key_Dish]		= new Texture("Resource\\Key_Dish_37_9.bmp", 1 , 1);
	m_pTexture[Texture::TextureNumber::Main_Rogo]		= new Texture("Resource\\Main_Rogo_528_168.bmp", 1 ,1 );
	m_pTexture[Texture::TextureNumber::Press_C]			= new Texture("Resource\\Press_C_Start_326_78.bmp", 1 , 1);

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

void Renderer::DrawSolidRect(Vector2& vecPos, Vector2& size, Vector2& spriteCount, Texture::TextureNumber eTextureNumber)
{
	m_pTexture[eTextureNumber]->SetTexture();
	Vector2 vec2SpriteCount = m_pTexture[eTextureNumber]->GetSpriteMaxCount();

	float fXStartPos = vec2SpriteCount.x == 1 ? 0.0f : spriteCount.x / (float)vec2SpriteCount.x;
	float fXEndPos =	(float)(spriteCount.x + 1) / (float)vec2SpriteCount.x;

	float fYStartPos = vec2SpriteCount.y == 1 ? 0.0f : spriteCount.y / (float)vec2SpriteCount.y;
	float fYEndPos = (float)(spriteCount.y + 1) / (float)vec2SpriteCount.y;

	glBegin(GL_QUADS);

		glTexCoord2f(fXStartPos, fYStartPos);

		glVertex2d(vecPos.x - size.x / 2, vecPos.y - size.y / 2);

		glTexCoord2f(fXStartPos, fYEndPos);

		glVertex2d(vecPos.x - size.x / 2, vecPos.y + size.y / 2);

		glTexCoord2f(fXEndPos, fYEndPos);

		glVertex2d(vecPos.x + size.x / 2, vecPos.y + size.y / 2);

		glTexCoord2f(fXEndPos, fYStartPos);

		glVertex2d(vecPos.x + size.x / 2, vecPos.y - size.y / 2);

	glEnd();

	glDisable(GL_TEXTURE_2D);

}