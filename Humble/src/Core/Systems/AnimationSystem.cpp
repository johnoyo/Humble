#include "AnimationSystem.h"

namespace HBL 
{
	void AnimationSystem::Start()
	{
		FUNCTION_PROFILE()

		Registry::Get().View<Component::Animation>().ForEach([&](Component::Animation& animation)
		{
			for (auto anim : animation.animations)
				anim.time = glfwGetTime();
		}).Run();
	}

	void AnimationSystem::Run(float dt)
	{
		Registry::Get().View<Component::Animation>().ForEach([&](Component::Animation& animation)
		{
			if (animation.Enabled)
			{
				for (auto& anim : animation.animations)
				{
					if (anim.Enabled)
					{
						double current_time = glfwGetTime();

						// Repeat texture swap every time step.
						if (current_time - anim.time >= anim.step)
						{
							if (anim.frames != 0)
							{
								// If animation has not ended yet, progress it.
								anim.start_coords.x++;
								anim.sprite->coords = anim.start_coords;
								anim.frames--;
							}
							else
							{
								// If its a looping animation, reset component state.
								if (anim.loop)
								{
									anim.frames = anim.cached_frames;
									anim.start_coords.x -= anim.frames;
									anim.sprite->coords = anim.start_coords;
								}
							}

							anim.time = current_time;
						}
					}
				}
			}
		}).Run();
	}

	void AnimationSystem::Clear()
	{
		FUNCTION_PROFILE();

		Registry::Get().ClearArray<Component::Animation>();
	}

	void AnimationSystem::PlayAnimation(Component::Animation& animation, int index)
	{
		animation.animations[index].Enabled = true;

		for (uint32_t i = 0; i < animation.animations.size(); i++)
		{
			if (i != index)
				animation.animations[i].Enabled = false;
		}
	}

	void AnimationSystem::StopAnimation(Component::Animation& animation, int index)
	{
		animation.animations[index].Enabled = false;
	}

	void AnimationSystem::ResetAnimation(Component::Animation& animation, int index, int frame)
	{
		int prev = animation.animations[index].frames;
		int diff = frame - prev;

		animation.animations[index].frames = frame;
		animation.animations[index].cached_frames = frame;

		animation.animations[index].start_coords.x -= diff;
		animation.animations[index].sprite->coords = animation.animations[index].start_coords;
	}
}
