#pragma once
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include <algorithm>

namespace HBL {

	class HBL_API ScriptingSystem {
	public:
		void Start(int current_level);
		void Run(int current_level);
		void Clear(int current_level);
	};

}
