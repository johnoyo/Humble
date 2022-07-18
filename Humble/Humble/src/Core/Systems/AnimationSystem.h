#pragma once

#include "../HumbleAPI.h"
#include "../ISystem.h"
#include "../Managers/TextureManager.h"

namespace HBL {

	class HBL_API AnimationSystem final : public ISystem {
	public:
		virtual void Start() override;
		virtual void Run() override;
		virtual void Clear() override;
	};

}