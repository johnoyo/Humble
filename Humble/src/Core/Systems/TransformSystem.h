#pragma once
#include "../Core.h"
#include "../GlobalArrays.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "SpriteRendererSystem.h"
#include "../ECS/IRegistrySystem.h"

namespace HBL 
{
	class HBL_API TransformSystem final : public IRegistrySystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;
	};

}
