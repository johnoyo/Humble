#include "GravitySystem.h"

namespace HBL {

	void GravitySystem::Start(float GravityForce, float thres)
	{
		FUNCTION_PROFILE();

		force = GravityForce;
		threshold = thres;

		Filter(entities, "Transform", "Gravity");
	}

	void GravitySystem::ResetGravity(float GravityForce, float thres)
	{
		FUNCTION_PROFILE();

		// Reset Gravity forces
		force = GravityForce;
		threshold = thres;

		for (uint32_t i = 0; i < Gravity.size(); i++) {
			Gravity.at(i).appliedForce = 0.0f;
		}
	}

	void GravitySystem::Run()
	{
		//ENGINE_PROFILE("GravitySystem::Run");

		For_Each([&](IEntity& entt)
		{
			Component::Gravity& gravity = GET_COMPONENT(Gravity, entt);
			Component::Transform& transfom = GET_COMPONENT(Transform, entt);

			if (gravity.Enabled && !gravity.isGrounded) {

				if (!gravity.collides)
					gravity.appliedForce += -0.1f * force;
				else
					gravity.appliedForce = -1.0f;

				transfom.position.y += 2.0f * gravity.appliedForce;
			}
			else if (gravity.Enabled && gravity.isGrounded) {
				if (gravity.appliedForce <= threshold)
					gravity.appliedForce = threshold;
			}
		});
	}

	void GravitySystem::Clear()
	{
		Gravity.clear();
	}

}
