#pragma once

#include "../HumbleAPI.h"
#include "../IRegistrySystem.h"

#include "../Core.h"
#include "../GlobalArrays.h"
#include "../Utilities.h"
#include "../Rendering/Renderer.h"
#include "../Managers/TextureManager.h"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace HBL
{
	class HBL_API SpriteRendererSystem final : public IRegistrySystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;

		void InitVertexBuffer();
	};
}