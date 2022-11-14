#include "GravitySystem.h"

namespace HBL {

	void GravitySystem::Start()
	{
		FUNCTION_PROFILE();
	}

	void GravitySystem::InitializeGravity(float gravityForce, float thres)
	{
		FUNCTION_PROFILE();

		// Initialize Gravity forces
		force = gravityForce;
		threshold = thres;
	}

	void GravitySystem::ResetGravity(float gravityForce, float thres)
	{
		FUNCTION_PROFILE();

		// Reset Gravity forces
		force = gravityForce;
		threshold = thres;

		Filter<Component::Transform, Component::Gravity>().ForEach([&](IEntity& entt)
		{
			Component::Gravity& gravity = Registry::Get().GetComponent<Component::Gravity>(entt);
			gravity.appliedForce = 0.0f;
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
					gravity.appliedForce += -0.1f * force * dt;
				else
					gravity.appliedForce = -1.0f;

				transfom.position.y += 2.0f * gravity.appliedForce * dt;
			}
			else if (gravity.Enabled && gravity.isGrounded)
			{
				if (gravity.appliedForce <= threshold)
					gravity.appliedForce = threshold;
			}
		}).Run();
	}

	void GravitySystem::Clear()
	{
		Registry::Get().ClearArray<Component::Gravity>();
	}

}
