#pragma once
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ISystem.h"
#include "../Managers/InputManager.h"

namespace HBL {

	class HBL_API GravitySystem final : public ISystem {
	public:
		void Start();
		void Run();
		void Clear();

		void InitializeGravity(float gravityForce, float thres);
		void ResetGravity(float gravityForce, float thres);
	private:
		float force;
		float threshold;
	};

}
