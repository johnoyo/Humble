#pragma once

#include "IScene.h"
#include "Systems.h"
#include "Utilities.h"
#include "HumbleAPI.h"
#include "ECS/ISystem.h"
#include "Managers/SceneManager.h"
#include "ECS/Registry.h"

#include <vector>

namespace HBL 
{
	class HBL_API Application 
	{
	public:
		Application(float width, float height, const std::string& name, bool full_screen, bool vSync, bool fixedTimeStep, float fixedUpdates = 60.0f);

		void AddScene(IScene* scene);
		void ManageScenes();
		void Start();

	private:
		std::vector<IScene*> m_Scenes;
		bool m_FixedTimeStep = false;

		void RegisterSystems();
		void RegisterArrays();

		void InitializeSystems();
		void RestartSystems();

		void UpdateFixedSystems(float dt);
		void UpdateSystems(float dt);

		void Clear();
		void Shutdown();

		float m_LastTime = 0.0f;
		float m_Timer = m_LastTime;
		float m_DeltaTime = 0.0f;
		int m_Frames = 0, m_Updates = 0;

		float m_LimitFPS = 1.0f / 60.0f;
		float m_FixedDeltaTime = 0.0f;
		int m_FixedUpdates = 0;
	};
}