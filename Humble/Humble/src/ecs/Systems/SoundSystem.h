#pragma once
#include "../../Renderer.h"
#include "../Header.h"
#include <irrKlang.h>

class SoundSystem {
public:
	void Start();
	void PlaySound(const std::string& source, bool playLooped = false, bool startPaused = false);
private:
	irrklang::ISoundEngine* SoundEngine;
};
