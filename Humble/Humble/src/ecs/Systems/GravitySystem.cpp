#include "GravitySystem.h"

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
		Entity::BaseEntity entt = entities.at(i);
		if (entt.Gravity != -1 && Gravity.at(entt.Gravity).Enabled && !Gravity.at(entt.Gravity).isGrounded) {

			if (!Gravity.at(entt.Gravity).collides) Gravity.at(entt.Gravity).appliedForce += -0.1f * force;
			else Gravity.at(entt.Gravity).appliedForce = -1.0f;

			Transform.at(entt.Transform).position.y += 2.0f * Gravity.at(entt.Gravity).appliedForce;
		}
		else if (entt.Gravity != -1 && Gravity.at(entt.Gravity).Enabled && Gravity.at(entt.Gravity).isGrounded) {
			if (Gravity.at(entt.Gravity).appliedForce <= threshold) Gravity.at(entt.Gravity).appliedForce = threshold;
		}
	}
}

