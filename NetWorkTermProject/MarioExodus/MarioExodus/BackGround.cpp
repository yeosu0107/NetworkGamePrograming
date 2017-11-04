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

	m_pObject[SkyNGround].SetSize(Vector2(962, 700));
	m_pObject[SkyNGround].SetPosition(Vector2(481, 350));

	if (m_nStage == 0) {
		m_pObject[Rogo].SetSize(Vector2(528, 168));
		m_pObject[Rogo].SetPosition(Vector2(481, 500));

		m_pObject[PressButton].SetSize(Vector2(326, 78));
		m_pObject[PressButton].SetPosition(Vector2(481, 300));
	}

}

void BackGround::Render()
{
	GetRenderer()->DrawSolidRect(m_pObject[SkyNGround].GetPosition(), m_pObject[SkyNGround].GetSize(), Vector2(0, 0), Texture::TextureNumber::BackSky);

	if (m_nStage == 0) {
		GetRenderer()->DrawSolidRect(m_pObject[Rogo].GetPosition(), m_pObject[Rogo].GetSize(), Vector2(0, 0), Texture::TextureNumber::Main_Rogo);
		GetRenderer()->DrawSolidRect(m_pObject[PressButton].GetPosition(), m_pObject[PressButton].GetSize(), Vector2(0, 0), Texture::TextureNumber::Press_C);
	}
}