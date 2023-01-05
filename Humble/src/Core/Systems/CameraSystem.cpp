#include "CameraSystem.h"

void HBL::CameraSystem::Start()
{
	Registry::Get().Filter<Component::Camera, Component::Transform>().ForEach([&](IEntity& entt)
	{
		Component::Camera& camera = Registry::Get().GetComponent<Component::Camera>(entt);

		if (camera.Enabled)
		{
			Component::Transform& transform = Registry::Get().GetComponent<Component::Transform>(entt);

			glm::mat4 tranform =
				glm::translate(glm::mat4(1.0f), transform.position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));

			camera.viewProjectionMatrix = camera.projection * glm::inverse(tranform);

			if (camera.primary)
				SceneManager::Get().SetMainCamera(entt);
		}
	}).Run();
}

void HBL::CameraSystem::Run(float dt)
{
	Registry::Get().Filter<Component::Camera, Component::Transform>().ForEach([&](IEntity& entt)
	{
		Component::Camera& camera = Registry::Get().GetComponent<Component::Camera>(entt);

		if (camera.Enabled && !camera.Static)
		{
			Component::Transform& transform = Registry::Get().GetComponent<Component::Transform>(entt);

			glm::mat4 tranform =
				glm::translate(glm::mat4(1.0f), transform.position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));

			camera.viewProjectionMatrix = camera.projection * glm::inverse(tranform);

			if (camera.primary)
				SceneManager::Get().SetMainCamera(entt);
		}
		else if (camera.Enabled && camera.Static)
		{
			if (camera.primary)
				SceneManager::Get().SetMainCamera(entt);
		}
	}).Run();
}

void HBL::CameraSystem::Clear()
{
	Registry::Get().ClearArray<Component::Camera>();
}
