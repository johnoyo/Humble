#pragma once
#include "HumbleAPI.h"

namespace HBL {

	class HBL_API IScript {
	public:
		IScript() {}
		virtual ~IScript() {}

		virtual void OnCreate() = 0;
		virtual void OnUpdate(float dt) = 0;
	};

}
