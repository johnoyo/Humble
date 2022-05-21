#include "SoundSystem.h"

void SoundSystem::Start()
{
	/* Init sound Engine */
	SoundEngine = irrklang::createIrrKlangDevice();
	if (!SoundEngine)
	{
		std::cout << "Error: Could not create Sound Engine" << std::endl;
	}
}

void SoundSystem::PlaySound(const std::string& source, bool playLooped, bool startPaused)
{
	SoundEngine->play2D(source.c_str(), playLooped, startPaused);
}
