#include "SoundManager.h"

namespace HBL {

	void SoundManager::IStart()
	{
		/* Init sound Engine */
		m_SoundEngine = irrklang::createIrrKlangDevice();
		if (!m_SoundEngine)
		{
			std::cout << "Error: Could not create Sound Engine" << std::endl;
		}
	}

	void SoundManager::IPlaySound(const std::string& source, bool playLooped, bool startPaused)
	{
		m_SoundEngine->play2D(source.c_str(), playLooped, startPaused);
	}

}
