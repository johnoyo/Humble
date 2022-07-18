#include "AnimationSystem.h"

namespace HBL {

	void AnimationSystem::Start()
	{
		FUNCTION_PROFILE()

		Filter(Globals::entities, "Animation");

		For_Each([&](IEntity& entt)
		{
			Component::Animation& anim = GET_COMPONENT(Animation, entt);

			anim.time = glfwGetTime();
		});
	}

	void AnimationSystem::Run()
	{
		For_Each([&](IEntity& entt)
		{
			Component::Animation& anim = GET_COMPONENT(Animation, entt);

			if (anim.Enabled)
			{
				double current_time = glfwGetTime();

				// Repeat texture swap every time step.
				if (current_time - anim.time >= anim.step)
				{
					if (anim.frames != 0)
					{
						// If animation has not ended yet, progress it.
						anim.material->coords.x++;
						anim.frames--;
					}
					else 
					{
						// If is looping animation, reset component state.
						if (anim.loop)
						{
							anim.frames = 5;
							anim.material->coords.x = 6.0f;
						}
					}

					anim.time = current_time;
				}

				float new_time = glfwGetTime();
				float frame_time = new_time - current_time;
				current_time = new_time;
			}
		});
	}

	void AnimationSystem::Clear()
	{
		FUNCTION_PROFILE();

		Clean();
		Globals::Animation.clear();
	}

}
