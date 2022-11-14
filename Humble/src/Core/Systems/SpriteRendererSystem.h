#pragma once

#include "../HumbleAPI.h"
#include "../ECS/ISystem.h"

#include "../Core.h"
#include "../GlobalArrays.h"
#include "../Utilities.h"
#include "../Rendering/Renderer.h"
#include "../Managers/TextureManager.h"
#include "../ECS/Group.h"
#include "../ECS/Components.h"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace HBL
{
	class HBL_API SpriteRendererSystem final : public ISystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;

		void InitVertexBuffer();
	};
}