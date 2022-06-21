#include "GravitySystem.h"

namespace HBL {

	void GravitySystem::Start(float GravityForce, float thres)
	{
		force = GravityForce;
		threshold = thres;
	}

	void GravitySystem::ResetGravity(float GravityForce, float thres)
	{
		//ENGINE_PROFILE("GravitySystem::ResetGravity");

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

		for (uint32_t i = 0; i < entities.size(); i++) {
			IEntity entt = entities.at(i);
			if (TRY_FIND_COMPONENT(Gravity, entt) && GET_COMPONENT(Gravity, entt).Enabled && !GET_COMPONENT(Gravity, entt).isGrounded) {

				if (!GET_COMPONENT(Gravity, entt).collides) 
					GET_COMPONENT(Gravity, entt).appliedForce += -0.1f * force;
				else 
					GET_COMPONENT(Gravity, entt).appliedForce = -1.0f;

				GET_COMPONENT(Transform, entt).position.y += 2.0f * GET_COMPONENT(Gravity, entt).appliedForce;
			}
			else if (TRY_FIND_COMPONENT(Gravity, entt) && GET_COMPONENT(Gravity, entt).Enabled && GET_COMPONENT(Gravity, entt).isGrounded) {
				if (GET_COMPONENT(Gravity, entt).appliedForce <= threshold) 
					GET_COMPONENT(Gravity, entt).appliedForce = threshold;
			}
		}
	}

	void GravitySystem::Clear()
	{
		Gravity.clear();
	}

}
