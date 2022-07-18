#pragma once
#include "HumbleAPI.h"

namespace HBL {

	class HBL_API IScript {
	public:
		IScript() {}
		virtual ~IScript() {}

		virtual void Init() = 0;
		virtual void Update() = 0;
	};

}
