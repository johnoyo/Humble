#pragma once

#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ECS/ISystem.h"
#include "../ECS/IEntity.h"
#include "../ECS/Registry.h"
#include "../ECS/Components.h"

#include "../Managers/SceneManager.h"

#include <algorithm>

namespace HBL 
{
	class HBL_API ScriptingSystem final : public ISystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;
	};

}
