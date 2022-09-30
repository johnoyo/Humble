#include "GravitySystem.h"

namespace HBL {

	void GravitySystem::Start()
	{
		FUNCTION_PROFILE();

		Filter({ "Transform", "Gravity" });
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

		Filter({ "Gravity" }).For_Each([&](IEntity& entt)
		{
			Component::Gravity& gravity = GET_COMPONENT(Gravity, entt);
			gravity.appliedForce = 0.0f;
		});
	}

	void GravitySystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		Filter({ "Transform", "Gravity" }).For_Each([&](IEntity& entt)
		{
			Component::Gravity& gravity = GET_COMPONENT(Gravity, entt);
			Component::Transform& transfom = GET_COMPONENT(Transform, entt);

			if (gravity.Enabled && !gravity.isGrounded)
			{
				if (!gravity.collides)
					gravity.appliedForce += -0.1f * force;
				else
					gravity.appliedForce = -1.0f;

				transfom.position.y += 2.0f * gravity.appliedForce * dt;
			}
			else if (gravity.Enabled && gravity.isGrounded)
			{
				if (gravity.appliedForce <= threshold)
					gravity.appliedForce = threshold;
			}
		});
	}

	void GravitySystem::Clear()
	{
		Clean();
		Globals::Gravity.clear();
	}

}
