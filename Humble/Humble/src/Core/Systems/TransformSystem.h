#pragma once
#include "../Core.h"
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "RenderingSystem.h"
#include "../ISystem.h"

namespace HBL {

	class HBL_API TransformSystem final : public ISystem {
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;
	};

}
