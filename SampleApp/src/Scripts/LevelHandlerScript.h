#pragma once

namespace HBL 
{
	class LevelHandlerScript final : public IScript
	{
	public:
		LevelHandlerScript() {}
		~LevelHandlerScript() {}

		virtual void OnCreate() override 
		{

		}

		virtual void OnUpdate(float dt) override 
		{
			if (InputManager::GetKeyPress(GLFW_KEY_K))
				SceneManager::Get().TriggerSceneChange();
		}

	};
}
