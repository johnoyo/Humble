#pragma once
#include "../Header.h"
#include "WindowSystem.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>


class CameraSystem {
public:
	CameraSystem() { }
	CameraSystem(float left, float right, float bottom, float top);

	void Start();
	void Run();

	void Follow(Entity::BaseEntity player, float offset);
	void Follow(Entity::BaseEntity player, float offset_x, float offset_y);
	void Follow_Horizontally(Entity::BaseEntity player, float offset_x);
	void Follow_Vertically(Entity::BaseEntity player, float offset_y);

	const glm::mat4& Get_Projection_Matrix() const { return m_Projection_Matrix; }
	const glm::mat4& Get_View_Matrix() const { return m_View_Matrix; }
	const glm::mat4& Get_View_Projection_Matrix() const { return m_View_Projection_Matrix; }

private:
	void Set_Position(const glm::vec3& position) { ecs.GetComponent<Component::Transform>(camera.Transform, Transform).position = position; Recalculate_View_Matrix(); }
	void Set_Position_x(float position) { ecs.GetComponent<Component::Transform>(camera.Transform, Transform).position.x = position; Recalculate_View_Matrix(); }
	void Set_Position_y(float position) { ecs.GetComponent<Component::Transform>(camera.Transform, Transform).position.y = position; Recalculate_View_Matrix(); }
	void Incr_Position(const glm::vec3& position) { ecs.GetComponent<Component::Transform>(camera.Transform, Transform).position.x += position.x;  ecs.GetComponent<Component::Transform>(camera.Transform, Transform).position.y += position.y; Recalculate_View_Matrix(); }

	void Recalculate_View_Matrix();

	glm::mat4 m_Projection_Matrix;
	glm::mat4 m_View_Matrix;
	glm::mat4 m_View_Projection_Matrix;
};