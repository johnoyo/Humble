#pragma once
#include "../Header.h"
#include "../Utilities.h"
#include "../Managers/InputManager.h"

namespace HBL {

	class GravitySystem {
	public:
		void Start(float gravityForce, float thres);
		void ResetGravity(float gravityForce, float thres);
		void Run();
		void Clear();
	private:
		float force;
		float threshold;
	};

}
