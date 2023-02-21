#pragma once

#include "../ECS/IEntity.h"
#include "../ECS/Registry.h"
#include "../ECS/Components.h"

#include <vector>
#include <string>

namespace HBL 
{
	class IScene;
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

		IScene* m_ActiveScene = nullptr;
		glm::vec2 m_WorldSize = { 0.0f, 0.0f };
		glm::vec2 m_SectorSize = { 0.0f, 0.0f };
		float m_TileSize = 30.0f;

		void LoadScene(uint32_t index);
		void LoadNextScene();
		bool IsSceneChanging();
		HBL::IEntity& GetMainCamera();
		void SetMainCamera(HBL::IEntity& mainCamera);
		uint32_t& GetCurrentScene();


	private:
		SceneManager() { }

		friend class Application;

		HBL::IEntity m_MainCamera;
		bool m_SceneChange = false;
		bool m_ArbitrarySceneChange = false;
		uint32_t m_CurrentScene = 0;
		uint32_t m_NextScene = 0;
	};
}
