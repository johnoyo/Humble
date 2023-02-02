#pragma once

#include "../HumbleAPI.h"
#include "../ECS/ISystem.h"
#include "../Managers/SceneManager.h"

namespace HBL
{
	class HBL_API CameraSystem final : public ISystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;
	};
}