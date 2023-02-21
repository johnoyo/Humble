#include "SoundManager.h"

namespace HBL 
{
    bool SoundManager::ISucceededOrWarn(const std::string& message, FMOD_RESULT result)
    {
        if (result != FMOD_OK) {
            std::cerr << message << ": " << result << " " << FMOD_ErrorString(result) << std::endl;
            return false;
        }
        return true;
    }

	void SoundManager::IStart()
	{
        // Create the main system object.
        result = FMOD::System_Create(&system);
        if (result != FMOD_OK)
        {
            printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            return;
        }

        // Initialize FMOD.
        result = system->init(512, FMOD_INIT_NORMAL, 0);    
        if (result != FMOD_OK)
        {
            printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            return;
        }

        // Create the channel group.
        result = system->createChannelGroup("inGameSoundEffects", &channelGroup);
        if (!ISucceededOrWarn("FMOD: Failed to create in-game sound effects channel group", result))
            return;
	}

    void SoundManager::IUpdate()
    {
        system->update();
    }

    void SoundManager::IClean()
    {
        for (auto const& sound : sounds)
        {
            sound.second->release();
        }
    }

    bool SoundManager::IExists(const std::string& soundName)
    {
        return (sounds.find(soundName) != sounds.end());
    }

    bool SoundManager::IChannelExists(const std::string& soundName)
    {
        return (channels.find(soundName) != channels.end());
    }

	void SoundManager::IPlay(const std::string& source, bool playLooped, bool startPaused)
	{
        // Create the sound if it does not exist.
        if (!IExists(source))
        {
            FMOD::Sound* sound = nullptr;
            result = system->createSound(source.c_str(), FMOD_DEFAULT, nullptr, &sound);
            if (!ISucceededOrWarn("FMOD: Failed to create sound", result))
                return;

            if (playLooped)
                sound->setMode(FMOD_LOOP_NORMAL);
            else
                sound->setMode(FMOD_LOOP_OFF);

            sounds[source] = sound;
        }

        // Play the sound.
        result = system->playSound(sounds[source], nullptr, startPaused, &channels[source]);

        if (!ISucceededOrWarn("FMOD: Failed to play sound", result))
            return;
	}

    void SoundManager::IStop(const std::string& source)
    {
        // Stop all sounds.
        if (IChannelExists(source))
        {
            result = channels[source]->stop();
            if (!ISucceededOrWarn("FMOD: Failed to stop sound", result))
                return;
        }
    }
}
