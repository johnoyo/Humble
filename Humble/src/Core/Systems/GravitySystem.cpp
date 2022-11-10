#include "GravitySystem.h"

namespace HBL {

	void GravitySystem::Start()
	{
		FUNCTION_PROFILE();

		Filter<Component::Gravity, Component::Transform>();
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

		ForEach([&](IEntity& entt)
		{
			Component::Gravity& gravity = Globals::s_Registry.GetComponent<Component::Gravity>(entt);
			gravity.appliedForce = 0.0f;
		}).Run();
	}

	void GravitySystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		ForEach([&](IEntity& entt)
		{
			Component::Gravity& gravity = Globals::s_Registry.GetComponent<Component::Gravity>(entt);
			Component::Transform& transfom = Globals::s_Registry.GetComponent<Component::Transform>(entt);

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
		Clean();
		Globals::s_Registry.GetArray<Component::Gravity>().clear();
	}

}
