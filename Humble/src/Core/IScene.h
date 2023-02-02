#pragma once

#include "HumbleAPI.h"
#include "ECS\IEntity.h"

namespace HBL 
{
	class HBL_API IScene 
	{
	public:
		IScene() = default;

		virtual void OnAttach() = 0;
		virtual void OnCreate() = 0;
		virtual void OnDetach() = 0;
	};
}
