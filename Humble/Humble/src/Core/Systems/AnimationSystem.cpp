#include "AnimationSystem.h"

namespace HBL {

	void AnimationSystem::Start()
	{
		FUNCTION_PROFILE()

		Filter(Globals::entities, "Animation");

		For_Each([&](IEntity& entt)
		{
			Component::Animation& animation = GET_COMPONENT(Animation, entt);

			for (auto anim : animation.animations)
				anim.time = glfwGetTime();
		});
	}

	void AnimationSystem::Run(float dt)
	{
		For_Each([&](IEntity& entt)
		{
			Component::Animation& animation = GET_COMPONENT(Animation, entt);

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
								anim.material->coords = anim.start_coords;
								anim.frames--;
							}
							else
							{
								// If is looping animation, reset component state.
								if (anim.loop)
								{
									anim.frames = anim.cached_frames;
									anim.start_coords.x -= anim.frames;
									anim.material->coords = anim.start_coords;
								}
							}

							anim.time = current_time;
						}

						double new_time = glfwGetTime();
						double frame_time = new_time - current_time;
						current_time = new_time;
					}
				}
			}
		});
	}

	void AnimationSystem::Clear()
	{
		FUNCTION_PROFILE();

		Clean();
		Globals::Animation.clear();
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
		FUNCTION_PROFILE();

		int prev = animation.animations[index].frames;
		int diff = frame - prev;

		animation.animations[index].frames = frame;
		animation.animations[index].cached_frames = frame;

		animation.animations[index].start_coords.x -= diff;
		animation.animations[index].material->coords = animation.animations[index].start_coords;
	}

}
