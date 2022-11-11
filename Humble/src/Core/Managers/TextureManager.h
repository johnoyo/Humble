#pragma once

#include "../HumbleAPI.h"
#include "../Core.h"
#include "../GlobalArrays.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../Systems/SpriteRendererSystem.h"
#include <vector>
#include <string>
#include <iostream>
#include "stb_image/stb_image.h"

namespace HBL {

	class HBL_API TextureManager 
	{
	public:
		TextureManager(const TextureManager&) = delete;

		static TextureManager& Get() 
		{
			static TextureManager instance;
			return instance;
		}

		static void InitTransparentTexture() { Get().IInitTransparentTexture(); }
		static void LoadTexture(const std::string& path) { Get().ILoadTexture(path); }
		static float Find(const std::string& path) { return Get().IFind(path); }

		static uint32_t* GetTextureSlot() { return Get().m_TextureSlot; }
		static std::vector<std::string>& GetTextureMap() { return Get().m_TextureMap; }
		static std::vector<glm::vec2>& GetTextureSize() { return Get().m_TextureSize; }
		static uint32_t& GetTextureIndex() { return Get().m_CurrentIndex; }

	private:
		TextureManager() {}

		uint32_t m_TextureSlot[32] = { 0 };

		uint32_t m_CurrentIndex = 0;
		std::vector<std::string> m_TextureMap;
		std::vector<glm::vec2> m_TextureSize;
		void IInitTransparentTexture();
		void ILoadTexture(const std::string& path);
		float IFind(const std::string& path);
	};

}
