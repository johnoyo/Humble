#include "GravitySystem.h"

namespace HBL {

	void GravitySystem::Start()
	{
		FUNCTION_PROFILE();
		
		View<Component::Gravity>().ForEach([&](Component::Gravity& gravity)
		{
			gravity.appliedForce = gravity.force;
		}).Run();
	}

	void GravitySystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		Filter<Component::Transform, Component::Gravity>().ForEach([&](IEntity& entt)
		{
			Component::Gravity& gravity = Registry::Get().GetComponent<Component::Gravity>(entt);
			Component::Transform& transfom = Registry::Get().GetComponent<Component::Transform>(entt);

			if (gravity.Enabled && !gravity.isGrounded)
			{
				if (!gravity.collides)
					gravity.appliedForce += -0.1f * gravity.force * dt;
				else
					gravity.appliedForce = -1.0f;

				transfom.position.y += 2.0f * gravity.appliedForce * dt;
			}
			else if (gravity.Enabled && gravity.isGrounded)
			{
				if (gravity.appliedForce <= gravity.threshold)
					gravity.appliedForce = gravity.threshold;
			}
		}).Run();
	}

	void GravitySystem::Clear()
	{
		Registry::Get().ClearArray<Component::Gravity>();
	}

}
