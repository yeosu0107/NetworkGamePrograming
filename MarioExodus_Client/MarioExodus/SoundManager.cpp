#include "stdafx.h"
#include "SoundManager.h"


SoundManager::SoundManager()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&m_pFmodSystem);
	if (result != FMOD_OK)
		std::cout << "FMOD System 생성에 실패했습니다." << std::endl;

	result = m_pFmodSystem->init(MaxSound, FMOD_INIT_NORMAL, NULL);
	if (result != FMOD_OK) {
		std::cout << "FMOD 초기화에 실패했습니다." << std::endl;
		result = FMOD_OK;
	}

	result = m_pFmodSystem->createSound("Resource\\Sound\\BGM.mp3"	, FMOD_LOOP_NORMAL	, NULL, &m_ppFmodSound[SoundType::BGMSound]);
	if (result != FMOD_OK) {
		std::cout << "BGM.mp3 FMOD Sound로딩에 실패했습니다." << std::endl;
		result = FMOD_OK;
	}

	result = m_pFmodSystem->createSound("Resource\\Sound\\Clear.mp3", FMOD_LOOP_OFF		, NULL, &m_ppFmodSound[SoundType::StageClearSound]);
	if (result != FMOD_OK) {
		std::cout << "Clear.mp3 FMOD Sound로딩에 실패했습니다." << std::endl;
		result = FMOD_OK;
	}

	result = m_pFmodSystem->createSound("Resource\\Sound\\Jump.wav"	, FMOD_LOOP_OFF		, NULL, &m_ppFmodSound[SoundType::JumpSound]);
	if (result != FMOD_OK) {
		std::cout << "Jump.wav FMOD Sound로딩에 실패했습니다." << std::endl;
		result = FMOD_OK;
	}

	result = m_pFmodSystem->createSound("Resource\\Sound\\Exit.wav"	, FMOD_LOOP_OFF		, NULL, &m_ppFmodSound[SoundType::ExitSound]);
	if (result != FMOD_OK) {
		std::cout << "Exit.wav FMOD Sound로딩에 실패했습니다." << std::endl;
		result = FMOD_OK;
	}
}

SoundManager::~SoundManager()
{
	for (int i = 0; i < MaxSound; ++i)
		m_ppFmodChannel[i]->stop();


}

void SoundManager::Play(SoundType index)
{
	bool IsPlay = true;
	m_ppFmodChannel[index]->isPlaying(&IsPlay);

	if (IsPlay)
		return;
	
	if (index == SoundType::StageClearSound)
		m_ppFmodChannel[SoundType::BGMSound]->stop();

	m_pFmodSystem->playSound(m_ppFmodSound[index], NULL, false, &m_ppFmodChannel[index]);
}

void SoundManager::Stop(SoundType index)
{
	m_ppFmodChannel[index]->stop();
}

void SoundManager::Update()
{
	bool result;
	for (int i = 0; i < MaxSound; ++i) {
		if (m_ppFmodChannel[i] == NULL)
			continue;

		m_ppFmodChannel[i]->isPlaying(&result);
		if (!result)
			m_ppFmodChannel[i]->stop();
	}

}

bool SoundManager::IsPlaying(SoundType index)
{
	bool result;
	m_ppFmodChannel[index]->isPlaying(&result);
	return result;
}
