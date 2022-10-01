#include "SoundManager.h"

namespace HBL {

	void SoundManager::IStart()
	{
		// Initialize sound Engine

		/*m_SoundEngine = irrklang::createIrrKlangDevice();
		if (!m_SoundEngine)
		{
			std::cout << "Error: Could not create Sound Engine" << std::endl;
		}*/

		m_Result = ma_engine_init(NULL, &m_SoundEngine);

		if (m_Result != MA_SUCCESS)
		{
			std::cout << "Error: Could not create Sound Engine." << std::endl;
		}
	}

    void data_callback(ma_device * pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
    {
        ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
        if (pDecoder == NULL) 
        {
            return;
        }

        /* Reading PCM frames will loop based on what we specified when called ma_data_source_set_looping(). */
        ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

        (void)pInput;
    }

    void PlaySoundLooping(const std::string& source)
    {
        ma_result result;
        ma_decoder decoder;
        ma_device_config deviceConfig;
        ma_device device;

        result = ma_decoder_init_file(source.c_str(), NULL, &decoder);
        if (result != MA_SUCCESS) {
            return;
        }

        // A decoder is a data source which means we just use ma_data_source_set_looping() to set the
        // looping state. We will read data using ma_data_source_read_pcm_frames() in the data callback.
        ma_data_source_set_looping(&decoder, MA_TRUE);

        deviceConfig = ma_device_config_init(ma_device_type_playback);
        deviceConfig.playback.format = decoder.outputFormat;
        deviceConfig.playback.channels = decoder.outputChannels;
        deviceConfig.sampleRate = decoder.outputSampleRate;
        deviceConfig.dataCallback = data_callback;
        deviceConfig.pUserData = &decoder;

        if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
            printf("Failed to open playback device.\n");
            ma_decoder_uninit(&decoder);
            return;
        }

        if (ma_device_start(&device) != MA_SUCCESS) {
            printf("Failed to start playback device.\n");
            ma_device_uninit(&device);
            ma_decoder_uninit(&decoder);
            return;
        }

        printf("Press Enter to quit...");
        getchar();

        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);

        return;
    }

	void SoundManager::IPlaySound(const std::string& source, bool playLooped, bool startPaused)
	{
		//m_SoundEngine->play2D(source.c_str(), playLooped, startPaused);

        if (playLooped)
        {
            std::thread audioThread(PlaySoundLooping, source);
            audioThread.detach();
        }
        else
		    ma_engine_play_sound(&m_SoundEngine, source.c_str(), NULL);
	}

}
