#pragma once

#include "../ECS/IEntity.h"
#include "../ECS/Registry.h"
#include "../ECS/Components.h"

#include <vector>
#include <string>

namespace HBL 
{
	class Application;

	class HBL_API SceneManager
	{
	public:
		SceneManager(const SceneManager&) = delete;

		static SceneManager& Get()
		{
			static SceneManager instance;
			return instance;
		}

		void TriggerSceneChange();
		HBL::IEntity& GetMainCamera();
		void SetMainCamera(HBL::IEntity& mainCamera);
		uint32_t& GetCurrentLevel();

	private:
		SceneManager() { }

		friend class Application;

		bool m_SceneChange = false;
		uint32_t m_CurrentLevel = 0;
		HBL::IEntity m_MainCamera;
	};
}
