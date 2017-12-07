#pragma once
#include "stdafx.h"

class SoundManager
{
private:
	FMOD::System* m_pFmodSystem;
	FMOD::Sound*  m_ppFmodSound[MaxSound];
	FMOD::Channel* m_ppFmodChannel[MaxSound];
	
public:
	SoundManager();
	~SoundManager();

public:
	void Play(SoundType index);
	void Stop(SoundType index);
	bool IsPlaying(SoundType index);
};

