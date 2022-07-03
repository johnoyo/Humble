#pragma once
#include "HumbleAPI.h"

namespace HBL {

	class HBL_API ScriptFunction {
	public:
		ScriptFunction() {}
		virtual ~ScriptFunction() {}

		virtual void Init() = 0;
		virtual void Update() = 0;
	};

}
