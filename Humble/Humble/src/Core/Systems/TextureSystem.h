#pragma once
#include "../Core.h"
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ISystem.h"
#include "RenderingSystem.h"
#include <vector>
#include <iostream>
#include "stb_image/stb_image.h"

namespace HBL {

	class HBL_API TextureSystem final : public ISystem {
	public:
		virtual void Start() override;
		virtual void Run() override;
		virtual void Clear() override;

	private:
		uint32_t texture_slot[32] = { 0 };

		uint32_t current_index = 0;
		std::vector<std::string> map;
		std::vector<glm::vec2> size;
		void Init_Transparent_Texture();
		void Load_Texture(const std::string& path);
		float Find(const std::string& path);

	};

}
