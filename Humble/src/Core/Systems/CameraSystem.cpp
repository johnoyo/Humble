#include "CameraSystem.h"

namespace HBL {

	void CameraSystem::Initialize(float left, float right, float bottom, float top)
	{
		m_View_Matrix = glm::mat4(1.0f);
		m_Projection_Matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_View_Projection_Matrix = m_Projection_Matrix * m_View_Matrix;
	}

	void CameraSystem::Recalculate_View_Matrix(IEntity& camera)
	{
		glm::mat4 tranform =
			glm::translate(glm::mat4(1.0f), Registry::Get().GetComponent<Component::Transform>(camera).position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));

		m_View_Matrix = glm::inverse(tranform);
		m_View_Projection_Matrix = m_Projection_Matrix * m_View_Matrix;
	}

	void CameraSystem::Create()
	{
		m_View_Projection_Matrix = m_Projection_Matrix * m_View_Matrix;
	}

	void CameraSystem::Follow(IEntity& camera, IEntity& player, float offset)
	{
		Set_Position_x(camera, Registry::Get().GetComponent<Component::Transform>(player).position.x + offset);
		Set_Position_y(camera, Registry::Get().GetComponent<Component::Transform>(player).position.y + offset);
		Recalculate_View_Matrix(camera);
	}

	void CameraSystem::Follow(IEntity& camera, IEntity& player, float offset_x, float offset_y)
	{
		Set_Position_x(camera, Registry::Get().GetComponent<Component::Transform>(player).position.x + offset_x);
		Set_Position_y(camera, Registry::Get().GetComponent<Component::Transform>(player).position.y + offset_y);
		Recalculate_View_Matrix(camera);
	}

	void CameraSystem::Follow_Horizontally(IEntity& camera, IEntity& player, float offset_x)
	{
		Set_Position_x(camera, Registry::Get().GetComponent<Component::Transform>(player).position.x + offset_x);
		Recalculate_View_Matrix(camera);
	}

	void CameraSystem::Follow_Vertically(IEntity& camera, IEntity& player, float offset_y)
	{
		Set_Position_y(camera, Registry::Get().GetComponent<Component::Transform>(player).position.y + offset_y);
		Recalculate_View_Matrix(camera);
	}

}
