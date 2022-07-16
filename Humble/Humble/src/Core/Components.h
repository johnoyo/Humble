#pragma once
#include <functional>
#include "glm/glm.hpp"
#include "ScriptFunction.h"
#include "HumbleAPI.h"

namespace HBL {

	namespace Component {

		struct HBL_API Transform {
			glm::vec3 position = { 0.0f, 0.0f, 0.0f };
			float rotation = 0.0f;
			glm::vec3 scale = { 10.0f, 10.0f, 10.0f };

			bool Static = false;
			int bufferIndex = -1;

			bool Enabled = true;
		};

		struct HBL_API CollisionBox {
			glm::vec3 bl = { 0.0f, 0.0f, 0.0f };
			glm::vec3 br = { 0.0f, 0.0f, 0.0f };
			glm::vec3 tl = { 0.0f, 0.0f, 0.0f };
			glm::vec3 tr = { 0.0f, 0.0f, 0.0f };

			bool Enabled = true;
		};

		struct HBL_API Material {
			std::string texture = "-";
			struct SubTexture {
				glm::vec2 coords = { -1.0f, -1.0f };
				glm::vec2 sprite_size = { -1.0f, -1.0f };
				std::string path = "-";
			} subTexture;
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

			bool Enabled = true;
		};

		/*struct HBL_API MaterialAtlas {
			struct SubTexture {
				glm::vec2 coords = { -1.0f, -1.0f };
				glm::vec2 sprite_size = { -1.0f, -1.0f };
				std::string path = "-";
			} subTexture;
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

			bool Enabled = true;
		};*/

		struct HBL_API Animation {
			std::string film = "";
			float step = 0.0f;

			bool Enabled = true;
		};

		struct HBL_API Gravity {
			bool collides = true;
			bool isGrounded = false;
			float appliedForce = 0.0f;

			bool Enabled = true;
		};

		struct HBL_API AudioSource {
			std::string audio = "";

			bool Enabled = true;
		};

		struct HBL_API Script {
			std::vector<HBL::ScriptFunction*> script;

			bool Enabled = true;
		};

		struct HBL_API Shadow {
			glm::vec4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
			float shadowDistance = 200000.0f;
			int parentBufferIndex = -1;
			int bufferIndex = -1;

			bool Enabled = true;
		};
	}

}
