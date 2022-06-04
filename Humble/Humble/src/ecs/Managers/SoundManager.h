#pragma once
#include "../../Core.h"
#include "../Header.h"
#include <irrKlang.h>

class SoundManager {
public:
	SoundManager(const SoundManager&) = delete;

	static SoundManager& Get() {
		static SoundManager instance;
		return instance;
	}

	static void Start() { Get().IStart(); }
	static void PlaySound(const std::string& source, bool playLooped = false, bool startPaused = false) { Get().IPlaySound(source, playLooped, startPaused); }

private:
	SoundManager() {}

	void IStart();
	void IPlaySound(const std::string& source, bool playLooped = false, bool startPaused = false);
	irrklang::ISoundEngine* m_SoundEngine = nullptr;
};
