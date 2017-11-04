#pragma once
#include "stdafx.h"
#include "Vector2.h"
#include "Texture.h"

class Renderer
{
public:
	Renderer() {};
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(Vector2& vecPos, int size);
	void DrawSolidRect(Vector2& vecPos, Vector2& size, Vector2& spriteCount, Texture::TextureNumber eTextureNumber);

private:
	void Initialize(int windowSizeX, int windowSizeY);
	
	bool m_Initialized = false;

	UINT m_WindowSizeX = 0;
	UINT m_WindowSizeY = 0;

	Texture** m_pTexture;
};

