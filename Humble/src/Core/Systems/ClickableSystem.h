#pragma once

#include "../HumbleAPI.h"
#include "../ECS/ISystem.h"
#include "../ECS/Registry.h"

#include "../Managers/InputManager.h"
#include "../Managers/SceneManager.h"

namespace HBL
{
	class HBL_API ClickableSystem final : public ISystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;
	private:
		bool Clicked(glm::vec3& position, glm::vec3& scale);
	};
}
