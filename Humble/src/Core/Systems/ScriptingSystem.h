#pragma once
#include "../GlobalArrays.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ECS/IRegistrySystem.h"
#include <algorithm>

namespace HBL 
{
	class HBL_API ScriptingSystem final : public IRegistrySystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;
	};

}
