#include "SceneManager.h"
#include "../Application.h"
#include "../IScene.h"

namespace HBL
{
	void SceneManager::TriggerSceneChange()
	{
		m_SceneChange = true;
	}

	HBL::IEntity& SceneManager::GetMainCamera()
	{
		return m_MainCamera;
	}

	void SceneManager::SetMainCamera(HBL::IEntity& mainCamera)
	{
		m_MainCamera = mainCamera;
	}

	uint32_t& SceneManager::GetCurrentLevel()
	{
		return m_CurrentLevel;
	}
}
