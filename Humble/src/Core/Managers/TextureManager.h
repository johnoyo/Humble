#pragma once

#include "../HumbleAPI.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ECS/IEntity.h"
#include "../ECS/Registry.h"
#include "../ECS/Components.h"
#include "../Systems/SpriteRendererSystem.h"

#include "stb_image/stb_image.h"

#include <vector>
#include <string>
#include <iostream>

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

		void InitTransparentTexture();
		void LoadTexture(const std::string& path);
		void LoadTexture(const std::string& name, void* data, float width, float height);
		float Find(const std::string& path, Component::SpriteRenderer* sprite = nullptr);

		uint32_t* GetTextureSlot() { return m_TextureSlot; }
		std::vector<std::string>& GetTextureMap() { return m_TextureMap; }
		std::vector<glm::vec2>& GetTextureSize() { return m_TextureSize; }
		uint32_t& GetTextureIndex() { return m_CurrentIndex; }

	private:
		TextureManager() {}

		uint32_t m_TextureSlot[32] = { 0 };

		uint32_t m_CurrentIndex = 0;
		std::vector<std::string> m_TextureMap;
		std::vector<glm::vec2> m_TextureSize;
	};

}
