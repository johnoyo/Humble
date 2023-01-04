#pragma once

#include "Group.h"
#include "IEntity.h"

#include "../HumbleAPI.h"

#include <functional>

namespace HBL 
{
	class HBL_API ISystem 
	{
	public:
		virtual void Start() = 0;
		virtual void Run(float dt) = 0;
		virtual void Clear() = 0;
	};
}