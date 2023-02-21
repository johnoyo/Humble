#include "SceneManager.h"
#include "../Application.h"
#include "../IScene.h"

namespace HBL
{
	void SceneManager::LoadNextScene()
	{
		m_SceneChange = true;
	}

	void SceneManager::LoadScene(uint32_t index)
	{
		m_SceneChange = true;
		m_ArbitrarySceneChange = true;
		m_NextScene = index;
	}

	bool SceneManager::IsSceneChanging()
	{
		return m_SceneChange;
	}

	HBL::IEntity& SceneManager::GetMainCamera()
	{
		return m_MainCamera;
	}

	void SceneManager::SetMainCamera(HBL::IEntity& mainCamera)
	{
		m_MainCamera = mainCamera;
	}

	uint32_t& SceneManager::GetCurrentScene()
	{
		return m_CurrentScene;
	}
}
