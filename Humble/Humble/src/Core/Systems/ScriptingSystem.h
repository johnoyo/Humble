#pragma once
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ISystem.h"
#include <algorithm>

namespace HBL {

	class HBL_API ScriptingSystem final : public ISystem {
	public:
		virtual void Start() override;
		virtual void Run() override;
		virtual void Clear() override;
	};

}
