#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
}

BackGround::BackGround(int stage, Renderer * pRend)
{
	InitBackGround(stage, pRend);
}


BackGround::~BackGround()
{
}

void BackGround::InitBackGround(int stage, Renderer* pRend) 
{
	SetRenderer(pRend);
	m_nStage = stage;

	m_pObject[SkyNGround].SetSize(Vector2(Screen_Width, Screen_Height));
	m_pObject[SkyNGround].SetPosition(Vector2(Screen_Width / 2, Screen_Height / 2));

	if (m_nStage == 0) {
		m_pObject[Rogo].SetSize(Vector2(528, 168));
		m_pObject[Rogo].SetPosition(Vector2(Screen_Width / 2, Screen_Height / 2 + 200));

		m_pObject[PressButton].SetSize(Vector2(326, 78));
		m_pObject[PressButton].SetPosition(Vector2(Screen_Width / 2, Screen_Height / 2));
	}

	else if (m_nStage == MaxStage - 1) {
		m_pObject[Rogo].SetSize(Vector2(528, 168));
		m_pObject[Rogo].SetPosition(Vector2(Screen_Width / 2, Screen_Height / 2 + 200));

	}

}

void BackGround::Render()
{
	GetRenderer()->DrawSolidRect(m_pObject[SkyNGround].GetPosition(), m_pObject[SkyNGround].GetSize(), Vector2(0, 0), Texture::TextureNumber::BackSky);

	if (m_nStage == 0) {
		GetRenderer()->DrawSolidRect(m_pObject[Rogo].GetPosition(), m_pObject[Rogo].GetSize(), Vector2(0, 0), Texture::TextureNumber::Main_Rogo);
	}
	else if (m_nStage == MaxStage - 1) {
		GetRenderer()->DrawSolidRect(m_pObject[Rogo].GetPosition(), m_pObject[Rogo].GetSize(), Vector2(0, 0), Texture::TextureNumber::GameOver);
	}
}