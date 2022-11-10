#pragma once

#include "../GlobalArrays.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../IRegistrySystem.h"
#include "../Managers/TextureManager.h"

namespace HBL {

	class HBL_API MaterialSystem final : public IRegistrySystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;
	};

}
