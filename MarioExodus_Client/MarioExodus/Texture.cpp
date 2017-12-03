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

	if ((fp = fopen(filename, "rb")) == NULL)					// 1. 열기 실패
	{
		printf("파일을 여는 데 실패했습니다.\n");
		return NULL;
	}
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)	// 2. 비트맵의 헤더 크기만큼 읽어오는데 성공해야 한다. 
	{
		printf("Error 2\n");
		fclose(fp);
		return NULL;
	}
	
	if (header.bfType != 'MB')									// 3. bfType = MB여야 BITMAP을 사용하겠다는 뜻이 된다. 
	{
		printf("Error 3\n");
		fclose(fp);
		return NULL;
	}

	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);		// 4. 이미지가 저장된곳의 시작위치 - 헤더의 크기 = 비트맵에 대한 정보가 담긴 데이터 길이

	if ((m_pBitInfo = (BITMAPINFO *)malloc(infosize)) == NULL)	// 5. BITMAPINFO데이터 크기만금 메모리를 할당 
	{
		printf("Error 4\n");
		fclose(fp);
		exit(0);
		return NULL;
	}

	if (fread(m_pBitInfo, 1, infosize, fp) < (unsigned int)infosize) // 6. 비트맵에 대한 정보가 담겨있는 부분을 읽어온다.
	{
		printf("Error 5\n");
		free(m_pBitInfo);
		fclose(fp);
		return NULL;
	}

	if ((bitsize = (m_pBitInfo)->bmiHeader.biSizeImage) == 0)		// 7. 비트맵의 크기를 못 읽어온 경우 직접 데이터 크기를 삽입.	
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
