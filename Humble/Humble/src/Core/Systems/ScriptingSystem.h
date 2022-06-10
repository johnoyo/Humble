#pragma once
#include "../Header.h"
#include "../Utilities.h"

namespace HBL {

	class ScriptingSystem {
	public:
		void Start(int current_level);
		void Run(int current_level);
		void Clear(int current_level);
	};

}