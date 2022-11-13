#pragma once

#include "../GlobalArrays.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ECS/IRegistrySystem.h"
#include "../Managers/InputManager.h"

namespace HBL 
{
	class HBL_API GravitySystem final : public IRegistrySystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;

		void InitializeGravity(float gravityForce, float thres);
		void ResetGravity(float gravityForce, float thres);
	private:
		float force;
		float threshold;
	};

}
