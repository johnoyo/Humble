#pragma once

#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ISystem.h"
#include "../Managers/TextureManager.h"

namespace HBL {

	class HBL_API MaterialSystem final : public ISystem {
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;
	};

}
