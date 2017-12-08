#include "stdafx.h"
#include "Time.h"


Time::Time() :
	m_dPreframeTime(::timeGetTime()),
	m_dCurframeTime(::timeGetTime()),
	m_fElapsedTime(0.0f),
	m_fAverageframeTime(0.0f),
	m_iCurrentframeRate(0),
	m_iCountframeRate(0),
	m_fTotalframeRate(0.0f),
	m_fTimeScale(0.001f),
	m_ffps(60.0f)
{
}

Time::~Time()
{
}

Time::Time(float fps) :
	m_dPreframeTime(::timeGetTime()),
	m_dCurframeTime(::timeGetTime()),
	m_fElapsedTime(0.0f),
	m_fAverageframeTime(0.0f),
	m_iCurrentframeRate(0),
	m_iCountframeRate(0),
	m_fTotalframeRate(0.0f),
	m_fTimeScale(0.001f),
	m_ffps(fps)
{}

void Time::Update(float fps)
{
	m_ffps = fps;
	m_dCurframeTime = ::timeGetTime();
	

	while ((float)(m_dCurframeTime - m_dPreframeTime) * m_fTimeScale < 1.0f / m_ffps) {
		m_dCurframeTime = ::timeGetTime();
	}
	
	m_fElapsedTime = (float)(m_dCurframeTime - m_dPreframeTime) * m_fTimeScale;
	m_fTotalframeRate += m_fElapsedTime;
	m_iCountframeRate++;

	if (m_fTotalframeRate > 1.0f) {
		m_iCurrentframeRate = m_iCountframeRate;
		m_iCountframeRate = 0;
		m_fTotalframeRate = 0.0f;
	}

	if (m_pEverageFrame.size() == MAX_EVERAGE_FRAME) {
		m_fAverageframeTime -= m_pEverageFrame.front();
		m_pEverageFrame.pop();
	}

	m_pEverageFrame.push(m_fElapsedTime);
	m_fAverageframeTime += m_fElapsedTime;

};

std::string Time::GetFrameTime()
{
	std::string sResult;

	sResult = "Mario Exodus (" + std::to_string(m_iCurrentframeRate) + " fps)";
	return sResult;
}
