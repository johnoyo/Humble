#pragma once

#include "glm/glm.hpp"
#include "../IScript.h"
#include "../HumbleAPI.h"

#include <functional>

namespace HBL 
{
	namespace Component 
	{
		struct HBL_API Tag
		{
			std::string tag = "";
		};

		struct HBL_API Transform 
		{
			glm::vec3 position = { 0.0f, 0.0f, 0.0f };
			float rotation = 0.0f;
			glm::vec3 scale = { 10.0f, 10.0f, 10.0f };

			bool Static = false;
			int bufferIndex = -1;

			bool Enabled = true;
		};

		struct HBL_API Camera
		{
			glm::mat4 projection = glm::mat4(1.0f);
			glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);

			bool primary = true;
			bool Static = false;

			bool Enabled = true;
		};

		struct HBL_API CollisionBox 
		{
			glm::vec3 bl = { 0.0f, 0.0f, 0.0f };
			glm::vec3 br = { 0.0f, 0.0f, 0.0f };
			glm::vec3 tl = { 0.0f, 0.0f, 0.0f };
			glm::vec3 tr = { 0.0f, 0.0f, 0.0f };

			bool Enabled = true;
		};

		struct HBL_API SpriteRenderer 
		{
			std::string texture = "-";
			glm::vec2 coords = { -1.0f, -1.0f };
			glm::vec2 spriteSize = { -1.0f, -1.0f };
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			void* pixelData = nullptr;
			int bufferIndex = -1;
			int layerIndex = -1;

			bool Enabled = true;
		};

		struct HBL_API ParalaxScrolling
		{
			float weight = 1.0f;
			bool Enabled = true;
		};

		struct HBL_API Clickable
		{
			std::function<void(void)> OnClick;
			bool Enabled = true;
		};

		struct HBL_API Animation 
		{
			struct Animations 
			{
				std::string name = "-";

				SpriteRenderer* sprite;

				glm::vec2 start_coords = { -1.0f, -1.0f };
				double time = 0.0;
				double step = 0.0;
				int frames = -1;
				int cached_frames = -1;
				bool loop = false;

				bool Enabled = true;
			};

			std::vector<Animations> animations;

			bool Enabled = true;
		};

		struct HBL_API Gravity 
		{
			bool collides = true;
			bool isGrounded = false;
			float force = 0.0f;
			float threshold = 0.0f;

			float appliedForce = 0.0f;

			bool Enabled = true;
		};

		struct HBL_API Script 
		{
			std::vector<HBL::IScript*> script;

			bool Enabled = true;
		};

		struct HBL_API Shadow 
		{
			IEntity* source = nullptr;
			glm::vec4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
			std::vector<glm::vec2> points;
			float shadowDistance = 200000.0f;
			int parentBufferIndex = -1;
			int bufferIndex = -1;

			bool Enabled = true;
		};

		struct HBL_API Text 
		{
			std::string text = "Sample text";
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			uint32_t lineLength = 32;

			glm::vec3 position = { 0.0f, 0.0f, 0.0f };
			float rotation = 0.0f;
			glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

			std::vector<uint32_t> bufferIndex;
			glm::vec2 sreenSpaceOffset = { 0.0f, 0.0f };
			bool screenSpace = false;

			bool Enabled = true;
		};
	}
}
