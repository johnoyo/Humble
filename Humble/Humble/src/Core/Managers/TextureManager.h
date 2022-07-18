#pragma once

#include "../HumbleAPI.h"
#include "../Core.h"
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../Systems/RenderingSystem.h"
#include <vector>
#include <string>
#include <iostream>
#include "stb_image/stb_image.h"

namespace HBL {

	class HBL_API TextureManager {
	public:
		TextureManager(const TextureManager&) = delete;

		static TextureManager& Get() {
			static TextureManager instance;
			return instance;
		}

		static void Init_Transparent_Texture() { Get().I_Init_Transparent_Texture(); }
		static void Load_Texture(const std::string& path) { Get().I_Load_Texture(path); }
		static float Find(const std::string& path) { return Get().I_Find(path); }

		static uint32_t* GetTextureSlot() { return Get().texture_slot; }
		static std::vector<std::string>& GetTextureMap() { return Get().map; }
		static std::vector<glm::vec2>& GetTextureSize() { return Get().size; }
		static uint32_t& GetTextureIndex() { return Get().current_index; }

	private:
		TextureManager() {}

		uint32_t texture_slot[32] = { 0 };

		uint32_t current_index = 0;
		std::vector<std::string> map;
		std::vector<glm::vec2> size;
		void I_Init_Transparent_Texture();
		void I_Load_Texture(const std::string& path);
		float I_Find(const std::string& path);
	};

}
