#pragma once
#include <functional>
#include "glm/glm.hpp"

namespace Component {

	struct Transform {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		float rotation = 0.0f;
		glm::vec3 scale = { 10.0f, 10.0f, 10.0f };

		bool Enabled = true;
		bool Static = false;
		int bufferIndex;
	};

	struct CollisionBox {
		glm::vec3 bl;
		glm::vec3 br;
		glm::vec3 tl;
		glm::vec3 tr;
		bool Enabled = true;
	};

	struct Material {
		std::string texture = "-";
		struct SubTexture {
			glm::vec2 coords = { -1.0f, -1.0f };
			glm::vec2 sprite_size = { -1.0f, -1.0f };
			std::string path = "-";
		} subTexture;
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		bool Enabled = true;
	};

	struct Animation {
		std::string film;
		float step;
		bool Enabled = true;;
	};

	struct Gravity {
		bool collides = true;
		bool isGrounded = false;
		float appliedForce = 0.0f;
		bool Enabled = true;
	};

	struct AudioSource {
		std::string audio;
		bool Enabled = true;
	};

	struct Health {
		int health = 100;
		bool Enabled = true;
	};

	struct Script {
		std::vector<std::function<void(float)>> init;
		std::vector<std::function<void(float)>> update;
		bool Enabled = true;
	};

	struct Shadow {
		glm::vec4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
		float shadowDistance = 2000.0f;
		int parentBufferIndex;
		int bufferIndex;
		bool Enabled = true;
	};
}
