#pragma once

#include "../HumbleAPI.h"
#include "../ECS/ISystem.h"
#include "../Managers/TextureManager.h"

namespace HBL 
{
	class HBL_API AnimationSystem final : public ISystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;

		void PlayAnimation(Component::Animation& animation, int index);
		void StopAnimation(Component::Animation& animation, int index);
		void ResetAnimation(Component::Animation& animation, int index, int frame);
	};

}