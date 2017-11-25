#include "stdafx.h"
#include "Texture.h"
#include "Utility.h"


Texture::Texture()
{
}

Texture::Texture(const char * filename, int iXSpriteCount, int iYSpriteCount)
{
	Vector2 vec2BitmapSize;

	// 비트맵의 X,Y축 크기
	vec2BitmapSize = LoadBMPImage(filename);

	// 스프라이트 X, Y사이즈
	m_iXMaxSpriteCount = iXSpriteCount;
	m_iYMaxSpriteCount = iYSpriteCount;
	
	// 스프라이트 1조각 당 크기
	m_iSpriteXSize = vec2BitmapSize.x / m_iXMaxSpriteCount;		
	m_iSpriteYSize = vec2BitmapSize.y / m_iYMaxSpriteCount;		
}



Texture::~Texture()
{
	delete(m_pBitmap);
}

Vector2 Texture::LoadBMPImage(const char* filename)
{
	FILE* fp;
	int bitsize, infosize;
	BITMAPFILEHEADER header;

	glGenTextures(1, &m_iTexture);
	glBindTexture(GL_TEXTURE_2D, m_iTexture);

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("Error 1\n");
		return NULL;
	}
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)
	{
		printf("Error 2\n");
		fclose(fp);
		return NULL;
	}

	if (header.bfType != 'MB')
	{
		printf("Error 3\n");
		fclose(fp);
		return NULL;
	}

	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);

	if ((m_pBitInfo = (BITMAPINFO *)malloc(infosize)) == NULL)
	{
		printf("Error 4\n");
		fclose(fp);
		exit(0);
		return NULL;
	}

	if (fread(m_pBitInfo, 1, infosize, fp) < (unsigned int)infosize)
	{
		printf("Error 5\n");
		free(m_pBitInfo);
		fclose(fp);
		return NULL;
	}

	if ((bitsize = (m_pBitInfo)->bmiHeader.biSizeImage) == 0)
	{
		bitsize = ((m_pBitInfo)->bmiHeader.biWidth *
			(m_pBitInfo)->bmiHeader.biBitCount + 7) / 8.0 *
			abs((m_pBitInfo)->bmiHeader.biHeight);
	}
	if ((m_pBitmap = (unsigned char *)malloc(bitsize)) == NULL)
	{
		printf("Error 6\n");
		free(m_pBitInfo);
		fclose(fp);
		return NULL;
	}

	if (fread(m_pBitmap, 1, bitsize, fp) < (unsigned int)bitsize)
	{
		printf("Error 7\n");
		free(m_pBitInfo);
		free(m_pBitmap);
		fclose(fp);
		return NULL;
	}

	for (int x = 3; x < bitsize; x += 4)
	{
		bool IsChannal = m_pBitmap[x - 1] == 215 && m_pBitmap[x - 2] == 100 && m_pBitmap[x - 3] == 100;

		if (!IsChannal)
		{
			if (isLittleEndian()) {
				GLubyte tmp;
				tmp = m_pBitmap[x - 3];
				m_pBitmap[x - 3] = m_pBitmap[x - 1];
				m_pBitmap[x - 1] = tmp;
			}
			m_pBitmap[x] = 255;
		}
	}
	printf("Success\n");
	fclose(fp);

	return Vector2(m_pBitInfo->bmiHeader.biWidth, m_pBitInfo->bmiHeader.biHeight);

}

void Texture::SetTexture()
{
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iSpriteXSize * m_iXMaxSpriteCount,

		m_iSpriteYSize * m_iYMaxSpriteCount, 0, GL_RGBA, GL_UNSIGNED_BYTE,

		GetBitmap());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_FLAT);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, GetTexture());
	
}
