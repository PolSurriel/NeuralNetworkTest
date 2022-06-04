#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string.h>
#include <unordered_map>
#include "Types.h"

#include <SDL_mixer.h>
#include <SDL_audio.h>

class AudioManager {
private:
	std::unordered_map<std::string, Mix_Music*> m_audioData;
	static AudioManager *audioManager;
	AudioManager();

	const float VOLUME_TO_MIX_VOLUME{ 1.28f };
	static const int LOOP_AUDIO{ -1 };

public:
	static const int MAX_VOLUME{ 100 };
	static const int MIN_VOLUME{ 0 };

	static AudioManager *Instance()
	{
		if (audioManager == nullptr) { audioManager = new AudioManager; }
		return audioManager;
	};

	~AudioManager();
	void DeleteResources();

	void PauseAudio();
	void ResumeAudio();
	int GetVolume();
	void SetVolume(const int &volumeAudio);
	void PlayAudio(const std::string &id, int times = LOOP_AUDIO);
	void LoadAudio(const std::string &id, const std::string &path);
};


