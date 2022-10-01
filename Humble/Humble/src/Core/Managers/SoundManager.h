#pragma once
#include "../Core.h"
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include <irrKlang.h>
#include "Miniaudio\miniaudio.h"
#include <thread>

namespace HBL {

	class HBL_API SoundManager {
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
		//irrklang::ISoundEngine* m_SoundEngine = nullptr;
		ma_engine m_SoundEngine;
		ma_decoder m_Decoder;
		ma_result m_Result;
	};

}
