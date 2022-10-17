#pragma once
#include "../GlobalArrays.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "WindowSystem.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>


namespace HBL {

	class HBL_API CameraSystem {
	public:
		void Initialize(float left, float right, float bottom, float top);
		void Create();

		void Follow(IEntity& camera, IEntity& player, float offset);
		void Follow(IEntity& camera, IEntity& player, float offset_x, float offset_y);
		void Follow_Horizontally(IEntity& camera, IEntity& player, float offset_x);
		void Follow_Vertically(IEntity& camera, IEntity& player, float offset_y);

		const glm::mat4& Get_Projection_Matrix() const { return m_Projection_Matrix; }
		const glm::mat4& Get_View_Matrix() const { return m_View_Matrix; }
		const glm::mat4& Get_View_Projection_Matrix() const { return m_View_Projection_Matrix; }

	private:
		void Set_Position(IEntity& camera, const glm::vec3& position) { GET_COMPONENT(Transform, camera).position = position; Recalculate_View_Matrix(camera); }
		void Set_Position_x(IEntity& camera, float position) { GET_COMPONENT(Transform, camera).position.x = position; Recalculate_View_Matrix(camera); }
		void Set_Position_y(IEntity& camera, float position) { GET_COMPONENT(Transform, camera).position.y = position; Recalculate_View_Matrix(camera); }
		void Incr_Position(IEntity& camera, IEntity& player, const glm::vec3& position) { GET_COMPONENT(Transform, camera).position.x += position.x;  GET_COMPONENT(Transform, camera).position.y += position.y; Recalculate_View_Matrix(camera); }

		void Recalculate_View_Matrix(IEntity& camera);

		glm::mat4 m_Projection_Matrix;
		glm::mat4 m_View_Matrix;
		glm::mat4 m_View_Projection_Matrix;
	};

}
