#pragma once
#include "../Header.h"
#include "../Utilities.h"
#include "../ISystem.h"
#include "../Managers/InputManager.h"
#include "../Managers/Entities.h"

namespace HBL {

	class GravitySystem final : public ISystem {
	public:
		void Start(float gravityForce, float thres);
		void Run();
		void Clear();

		void ResetGravity(float gravityForce, float thres);
	private:
		float force;
		float threshold;
	};

}
