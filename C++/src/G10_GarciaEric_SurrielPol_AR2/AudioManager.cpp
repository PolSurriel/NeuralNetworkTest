#include "AudioManager.h"

AudioManager::AudioManager() {
	// --- INIT Music---
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		throw "Unable to initialize SDL_mixer audio systems";
	}
}

AudioManager::~AudioManager() {
	DeleteResources();
	Mix_CloseAudio();
	Mix_Quit();
}

void AudioManager::DeleteResources() {
	for (auto &a : m_audioData) Mix_FreeMusic(a.second), a.second = nullptr;
}

void AudioManager::PauseAudio() {
	Mix_PauseMusic();
}

void AudioManager::ResumeAudio() {
	Mix_ResumeMusic();
}

void AudioManager::SetVolume(const int &volumeAudio) {
	Mix_VolumeMusic(volumeAudio * VOLUME_TO_MIX_VOLUME);
}

int AudioManager::GetVolume() {
	return Mix_VolumeMusic(-1);
}

void AudioManager::PlayAudio(const std::string &id, int times) {
	Mix_PlayMusic(m_audioData[id], times);
}

void AudioManager::LoadAudio(const std::string &id, const std::string &path) {
	Mix_Music *soundtrack{ Mix_LoadMUS(path.c_str()) };
	if (!soundtrack) throw "Unable to load the Mix_Music soundtrack";

	m_audioData[id] = soundtrack;
}

AudioManager* AudioManager::audioManager = nullptr;
