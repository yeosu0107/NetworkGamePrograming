#pragma once
#pragma warning (disable : 4996)
#include "Vector2.h"

class Texture
{
public:
	static enum TextureNumber {
		Mario_Sprite, Mario_None, Marios_Number, BackSky,
		Block, Door, Key, Key_Dish, Main_Rogo, Press_C, Wall, GameOver
	};
private:
	BITMAPINFO* m_pBitInfo;
	GLubyte* m_pBitmap;
	GLuint m_iTexture;

	UINT m_iXMaxSpriteCount;
	UINT m_iYMaxSpriteCount;
	UINT m_iSpriteXSize;
	UINT m_iSpriteYSize;

public:
	Texture();
	Texture(const char* filename, int iXSpriteCount, int iYSpriteCount);
	~Texture();

	void operator=(Texture& other) {
		m_pBitInfo = other.m_pBitInfo;
		m_pBitmap = other.m_pBitmap;
		m_iTexture = other.m_iTexture;
		m_iXMaxSpriteCount = other.m_iXMaxSpriteCount;
		m_iYMaxSpriteCount = other.m_iYMaxSpriteCount;
		m_iSpriteXSize = other.m_iSpriteXSize;
		m_iSpriteYSize = other.m_iSpriteYSize;
	};

public:
	Vector2 LoadBMPImage(const char* filename);
	Vector2 GetSpriteMaxCount() { return Vector2(m_iXMaxSpriteCount, m_iYMaxSpriteCount); }

	void SetTexture();
	GLuint GetTexture() const { return m_iTexture; }
	GLubyte* GetBitmap() const { return m_pBitmap; }
};

