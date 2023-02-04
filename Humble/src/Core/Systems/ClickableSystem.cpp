#include "ClickableSystem.h"

namespace HBL
{
	void ClickableSystem::Start()
	{
		Registry::Get().Group<Component::Transform, Component::Clickable>().ForEach([&](IEntity& entt)
		{
			// Empty.
		}).Run();
	}

	void ClickableSystem::Run(float dt)
	{
		Registry::Get().Group<Component::Transform, Component::Clickable>().ForEach([&](IEntity& entt)
		{
			if (InputManager::GetKeyPress(GLFW_MOUSE_BUTTON_LEFT))
			{
				Component::Transform& transform = Registry::Get().GetComponent<Component::Transform>(entt);
				Component::Clickable& clickable = Registry::Get().GetComponent<Component::Clickable>(entt);

				if (clickable.Enabled && transform.Enabled)
				{
					if (Clicked(transform.position, transform.scale))
					{
						clickable.OnClick();
					}
				}
			}
		}).Run();
	}

	void ClickableSystem::Clear()
	{
		Registry::Get().ClearArray<Component::Clickable>();
	}

	bool ClickableSystem::Clicked(glm::vec3& position, glm::vec3& scale)
	{
		glm::vec2& mousePosition = InputManager::GetMousePosition();
		glm::vec3& cameraPosition = Registry::Get().GetComponent<Component::Transform>(SceneManager::Get().GetMainCamera()).position;
		glm::vec2 targetPosition = { position.x + (-cameraPosition.x), position.y + (-cameraPosition.y) };

		if (mousePosition.x <= targetPosition.x + scale.x / 2.f && mousePosition.x >= targetPosition.x - scale.x / 2.f &&
			mousePosition.y <= targetPosition.y + scale.y / 2.f && mousePosition.y >= targetPosition.y - scale.y / 2.f)
		{
			return true;
		}

		return false;
	}
}
