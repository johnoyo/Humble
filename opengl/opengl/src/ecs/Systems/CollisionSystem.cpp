#include "CollisionSystem.h"

void CollisionSystem::Start()
{
	unsigned int i = 0;
	for (i = 0; i < entities.size(); i++) {
		if (TRY_FIND_COMPONENT(CollisionBox, entities.at(i)) && TRY_FIND_COMPONENT(Transform, entities.at(i))) {
			CollisionBox.at(entities.at(i).CollisionBox).tl.x = Transform.at(entities.at(i).Transform).position.x - Transform.at(entities.at(i).Transform).scale.x / 2.0f;
			CollisionBox.at(entities.at(i).CollisionBox).tl.y = Transform.at(entities.at(i).Transform).position.y + Transform.at(entities.at(i).Transform).scale.y / 2.0f;

			CollisionBox.at(entities.at(i).CollisionBox).tr.x = Transform.at(entities.at(i).Transform).position.x + Transform.at(entities.at(i).Transform).scale.x / 2.0f;
			CollisionBox.at(entities.at(i).CollisionBox).tr.y = Transform.at(entities.at(i).Transform).position.y + Transform.at(entities.at(i).Transform).scale.y / 2.0f;

			CollisionBox.at(entities.at(i).CollisionBox).br.x = Transform.at(entities.at(i).Transform).position.x + Transform.at(entities.at(i).Transform).scale.x / 2.0f;
			CollisionBox.at(entities.at(i).CollisionBox).br.y = Transform.at(entities.at(i).Transform).position.y - Transform.at(entities.at(i).Transform).scale.y / 2.0f;

			CollisionBox.at(entities.at(i).CollisionBox).bl.x = Transform.at(entities.at(i).Transform).position.x - Transform.at(entities.at(i).Transform).scale.x / 2.0f;
			CollisionBox.at(entities.at(i).CollisionBox).bl.y = Transform.at(entities.at(i).Transform).position.y - Transform.at(entities.at(i).Transform).scale.y / 2.0f;
		}
	}
}

void CollisionSystem::Run0(VertexBuffer& buffer)
{
	// update collision boxes and move the Transform of non-static objects
	for (unsigned int i = 0; i < entities.size(); i++) {
		if (entities.at(i).Transform != -1 && Transform.at(entities.at(i).Transform).Static == false) {
			Entity::BaseEntity entt = entities.at(i);

			glm::vec3 tr = Transform.at(entt.Transform).position;
			glm::vec3 sc = Transform.at(entt.Transform).scale;

			// move player on x-axis
			buffer.Update_PositionX_On_Quad(Transform.at(entt.Transform).bufferIndex, Transform.at(entt.Transform));

			// update collision box on x-axis
			if (entities.at(i).CollisionBox != -1) {
				CollisionBox.at(entt.CollisionBox).tl.x = tr.x - sc.x / 2.0f;
				CollisionBox.at(entt.CollisionBox).tr.x = tr.x + sc.x / 2.0f;
				CollisionBox.at(entt.CollisionBox).br.x = tr.x + sc.x / 2.0f;
				CollisionBox.at(entt.CollisionBox).bl.x = tr.x - sc.x / 2.0f;
			}

			// collision check on x-axis
			if(entities.at(i).CollisionBox != -1) Check_For_Collisions(entt, entt.CollisionBox, buffer, X_AXIS);

			// move player on y-axis
			buffer.Update_PositionY_On_Quad(Transform.at(entt.Transform).bufferIndex, Transform.at(entt.Transform));

			// update collision box on y-axis
			if (entities.at(i).CollisionBox != -1) {
				CollisionBox.at(entt.CollisionBox).tl.y = tr.y + sc.y / 2.0f;
				CollisionBox.at(entt.CollisionBox).tr.y = tr.y + sc.y / 2.0f;
				CollisionBox.at(entt.CollisionBox).br.y = tr.y - sc.y / 2.0f;
				CollisionBox.at(entt.CollisionBox).bl.y = tr.y - sc.y / 2.0f;
			}

			// collision check on y-axis
			if (entities.at(i).CollisionBox != -1) Check_For_Collisions(entt, entt.CollisionBox, buffer, Y_AXIS);
		}
	}
}

void CollisionSystem::Run(VertexBuffer& buffer)
{
	//ENGINE_PROFILE("CollisionSystem::Run");

	// update collision boxes of non-static objects
	for (unsigned int i = 0; i < entities.size(); i++) {
		if (entities.at(i).Transform != -1 && Transform.at(entities.at(i).Transform).Static == false) {
			Entity::BaseEntity entt = entities.at(i);

			glm::vec3 tr = Transform.at(entt.Transform).position;
			glm::vec3 sc = Transform.at(entt.Transform).scale;

			// update collision box on x-axis
			if (entities.at(i).CollisionBox != -1) {
				CollisionBox.at(entt.CollisionBox).tl.x = tr.x - sc.x / 2.0f;
				CollisionBox.at(entt.CollisionBox).tr.x = tr.x + sc.x / 2.0f;
				CollisionBox.at(entt.CollisionBox).br.x = tr.x + sc.x / 2.0f;
				CollisionBox.at(entt.CollisionBox).bl.x = tr.x - sc.x / 2.0f;
			}

			// collision check on x-axis
			if (entities.at(i).CollisionBox != -1) Check_For_Collisions(entt, entt.CollisionBox, buffer, X_AXIS);
			
			// update collision box on y-axis
			if (entities.at(i).CollisionBox != -1) {
				CollisionBox.at(entt.CollisionBox).tl.y = tr.y + sc.y / 2.0f;
				CollisionBox.at(entt.CollisionBox).tr.y = tr.y + sc.y / 2.0f;
				CollisionBox.at(entt.CollisionBox).br.y = tr.y - sc.y / 2.0f;
				CollisionBox.at(entt.CollisionBox).bl.y = tr.y - sc.y / 2.0f;
			}

			// collision check on y-axis
			if (entities.at(i).CollisionBox != -1) Check_For_Collisions(entt, entt.CollisionBox, buffer, Y_AXIS);
			
		}
	}
}

void CollisionSystem::Clear()
{
}

bool CollisionSystem::CollisionBetween(Entity::BaseEntity e0, Entity::BaseEntity e1, VertexBuffer& buffer)
{
	bool collision = false;

	collision = check_corner_br_tl_bool(CollisionBox.at(e0.CollisionBox).br, CollisionBox.at(e1.CollisionBox).tl, CollisionBox.at(e1.CollisionBox).br);
	if (collision) return true;

	collision = check_corner_tr_bl_bool(CollisionBox.at(e0.CollisionBox).tr, CollisionBox.at(e1.CollisionBox).bl, CollisionBox.at(e1.CollisionBox).tr);
	if (collision) return true;

	collision = check_corner_tl_br_bool(CollisionBox.at(e0.CollisionBox).tl, CollisionBox.at(e1.CollisionBox).br, CollisionBox.at(e1.CollisionBox).tl);
	if (collision) return true;

	collision = check_corner_bl_tr_bool(CollisionBox.at(e0.CollisionBox).bl, CollisionBox.at(e1.CollisionBox).tr, CollisionBox.at(e1.CollisionBox).bl);
	if (collision) return true;

	return false;
}

bool CollisionSystem::check_corner_br_tl_bool(glm::vec3 p_br, glm::vec3 e_tl, glm::vec3 e_br) {
	if (p_br.y <= e_tl.y && p_br.x >= e_tl.x && p_br.y >= e_br.y && p_br.x <= e_br.x)
		return true;
	return false;
}

bool CollisionSystem::check_corner_tr_bl_bool(glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_tr) {
	if (p_tr.y >= e_bl.y && p_tr.x >= e_bl.x && p_tr.y <= e_tr.y && p_tr.x <= e_tr.x)
		return true;
	return false;
}

bool CollisionSystem::check_corner_tl_br_bool(glm::vec3 p_tl, glm::vec3 e_br, glm::vec3 e_tl) {
	if (p_tl.y >= e_br.y && p_tl.x <= e_br.x && p_tl.y <= e_tl.y && p_tl.x >= e_tl.x)
		return true;
	return false;
}

bool CollisionSystem::check_corner_bl_tr_bool(glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_bl) {
	if (p_bl.y <= e_tr.y && p_bl.x <= e_tr.x && p_bl.y >= e_bl.y && p_bl.x >= e_bl.x)
		return true;
	return false;
}

void CollisionSystem::change_position_x(Entity::BaseEntity& p, VertexBuffer& buffer) {

	glm::vec3 tr = Transform.at(p.Transform).position;
	glm::vec3 sc = Transform.at(p.Transform).scale;

	// update collision box on x-axis
	CollisionBox.at(p.CollisionBox).tl.x = tr.x - sc.x / 2.0f;
	CollisionBox.at(p.CollisionBox).tr.x = tr.x + sc.x / 2.0f;
	CollisionBox.at(p.CollisionBox).br.x = tr.x + sc.x / 2.0f;
	CollisionBox.at(p.CollisionBox).bl.x = tr.x - sc.x / 2.0f;

	// move player on x-axis
	entities_to_be_updated.push_back(p.Transform);
	buffer.Update_PositionX_On_Quad(Transform.at(p.Transform).bufferIndex, Transform.at(p.Transform));
}

void CollisionSystem::change_position_y(Entity::BaseEntity& p, VertexBuffer& buffer) {

	glm::vec3 tr = Transform.at(p.Transform).position;
	glm::vec3 sc = Transform.at(p.Transform).scale;

	// update collision box on y-axis
	CollisionBox.at(p.CollisionBox).tl.y = tr.y + sc.y / 2.0f;
	CollisionBox.at(p.CollisionBox).tr.y = tr.y + sc.y / 2.0f;
	CollisionBox.at(p.CollisionBox).br.y = tr.y - sc.y / 2.0f;
	CollisionBox.at(p.CollisionBox).bl.y = tr.y - sc.y / 2.0f;

	// move player on y-axis
	entities_to_be_updated.push_back(p.Transform);
	buffer.Update_PositionY_On_Quad(Transform.at(p.Transform).bufferIndex, Transform.at(p.Transform));
}

bool CollisionSystem::check_corner_br_tl(VertexBuffer& buffer, Entity::BaseEntity& p, glm::vec3 p_br, glm::vec3 e_tl, glm::vec3 e_br, int axis)
{
	if (p_br.y < e_tl.y && p_br.x > e_tl.x && p_br.y > e_br.y && p_br.x < e_br.x) {
		//std::cout << "bottom right\n";
		if (axis == X_AXIS) {
			Transform.at(p.Transform).position.x += e_tl.x - p_br.x;
			change_position_x(p, buffer);
		}
		else if (axis == Y_AXIS) {
			Transform.at(p.Transform).position.y += e_tl.y - p_br.y;
			change_position_y(p, buffer);
		}
		return true;
	}
	return false;
}

bool CollisionSystem::check_corner_tr_bl(VertexBuffer& buffer, Entity::BaseEntity& p, glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_tr, int axis)
{
	if (p_tr.y > e_bl.y && p_tr.x > e_bl.x && p_tr.y < e_tr.y && p_tr.x < e_tr.x) {
		//std::cout << "top right\n";
		if (axis == X_AXIS) {
			Transform.at(p.Transform).position.x += e_bl.x - p_tr.x;
			change_position_x(p, buffer);
		}
		else if (axis == Y_AXIS) {
			//Transform.at(p.Transform).position.y += e_bl.y - 0.5f - p_tr.y;
			Transform.at(p.Transform).position.y += e_bl.y - p_tr.y;
			change_position_y(p, buffer);
		}
		return true;
	}
	return false;
}

bool CollisionSystem::check_corner_tl_br(VertexBuffer& buffer, Entity::BaseEntity& p, glm::vec3 p_tl, glm::vec3 e_br, glm::vec3 e_tl, int axis)
{
	if (p_tl.y > e_br.y && p_tl.x < e_br.x && p_tl.y < e_tl.y && p_tl.x > e_tl.x) {
		//std::cout << "top left\n";
		if (axis == X_AXIS) {
			Transform.at(p.Transform).position.x += e_br.x - p_tl.x;
			change_position_x(p, buffer);
		}
		else if (axis == Y_AXIS) {
			//Transform.at(p.Transform).position.y += e_br.y - 0.5f - p_tl.y;
			Transform.at(p.Transform).position.y += e_br.y - p_tl.y;
			change_position_y(p, buffer);
		}
		return true;
	}
	return false;
}

bool CollisionSystem::check_corner_bl_tr(VertexBuffer& buffer, Entity::BaseEntity& p, glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_bl, int axis)
{
	if (p_bl.y < e_tr.y && p_bl.x < e_tr.x && p_bl.y > e_bl.y && p_bl.x > e_bl.x) {
		//std::cout << "bottom left\n";
		if (axis == X_AXIS) {
			Transform.at(p.Transform).position.x += e_tr.x - p_bl.x;
			change_position_x(p, buffer);
		}
		else if (axis == Y_AXIS) {
			Transform.at(p.Transform).position.y += e_tr.y - p_bl.y;
			change_position_y(p, buffer);
		}
		return true;
	}
	return false;
}

bool CollisionSystem::check_side_l_r(VertexBuffer& buffer, Entity::BaseEntity& p, glm::vec3 p_br, glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_tl, glm::vec3 e_tr, int axis)
{
	if (p_br.y <= e_bl.y && p_tr.y >= e_tl.y && p_br.x > e_bl.x && p_tr.x > e_tl.x && p_tr.x < e_tr.x) {
		if (axis == X_AXIS) {
			//std::cout << "left to right\n";
			Transform.at(p.Transform).position.x += e_bl.x - p_br.x;
			change_position_x(p, buffer);
		}

		return true;
	}
	return false;
}

bool CollisionSystem::check_side_r_l(VertexBuffer& buffer, Entity::BaseEntity& p, glm::vec3 p_tl, glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_br, glm::vec3 e_bl, int axis)
{
	if (p_tl.y >= e_tr.y && p_bl.y <= e_br.y && p_tl.x < e_tr.x && p_bl.x < e_br.x && p_tl.x > e_bl.x) {
		if (axis == X_AXIS) {
			//std::cout << "right to left\n";
			Transform.at(p.Transform).position.x += e_br.x - p_bl.x;
			change_position_x(p, buffer);
		}

		return true;
	}
	return false;
}

bool CollisionSystem::check_side_b_t(VertexBuffer& buffer, Entity::BaseEntity& p, glm::vec3 p_tl, glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_br, glm::vec3 e_tl, int axis)
{
	if (p_tl.x <= e_bl.x && p_tl.y > e_bl.y && p_tr.x >= e_br.x && p_tr.y > e_br.y && p_tl.y < e_tl.y) {
		if (axis == Y_AXIS) {
			//std::cout << "bottom to top\n";
			//Transform.at(p.Transform).position.y += e_bl.y - 0.5f - p_tl.y;
			Transform.at(p.Transform).position.y += e_bl.y - p_tl.y;
			change_position_y(p, buffer);
		}

		return true;
	}
	return false;
}

bool CollisionSystem::check_side_t_b(VertexBuffer& buffer, Entity::BaseEntity& p, glm::vec3 p_br, glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_tl, glm::vec3 e_bl, int axis)
{
	if (p_bl.x <= e_tl.x && p_bl.y < e_tl.y && p_br.y < e_tr.y && p_br.x >= e_tr.x && p_bl.y > e_bl.y) {
		if (axis == Y_AXIS) {
			//std::cout << "top to bottom\n";
			Transform.at(p.Transform).position.y += e_tl.y - p_bl.y;
			change_position_y(p, buffer);
		}

		return true;
	}
	return false;
}

void CollisionSystem::Check_For_Collisions(Entity::BaseEntity& p, int collisionBox, VertexBuffer& buffer, int axis)
{
	unsigned int i = 0;
	if (p.Gravity != -1/* && axis == Y_AXIS*/) {
		Gravity.at(p.Gravity).collides = true;
		Gravity.at(p.Gravity).isGrounded = false;
	}
	for (i = 0; i < CollisionBox.size(); i++) {
		bool tmp = false;
		if (i != collisionBox && CollisionBox.at(i).Enabled) {

			tmp = check_corner_br_tl(buffer, p, CollisionBox.at(collisionBox).br, CollisionBox.at(i).tl, CollisionBox.at(i).br, axis);
			if (tmp != false) {
				if (p.Gravity != -1/* && axis == Y_AXIS*/) Gravity.at(p.Gravity).isGrounded = true;
				return;
			}

			tmp = check_corner_tr_bl(buffer, p, CollisionBox.at(collisionBox).tr, CollisionBox.at(i).bl, CollisionBox.at(i).tr, axis);
			if (tmp != false) {
				return;
			}

			tmp = check_corner_tl_br(buffer, p, CollisionBox.at(collisionBox).tl, CollisionBox.at(i).br, CollisionBox.at(i).tl, axis);
			if (tmp != false) {
				return;
			}

			tmp = check_corner_bl_tr(buffer, p, CollisionBox.at(collisionBox).bl, CollisionBox.at(i).tr, CollisionBox.at(i).bl, axis);
			if (tmp != false) {
				if (p.Gravity != -1/* && axis == Y_AXIS*/) Gravity.at(p.Gravity).isGrounded = true;
				return;
			}

			tmp = check_side_l_r(buffer, p, CollisionBox.at(collisionBox).br, CollisionBox.at(collisionBox).tr, CollisionBox.at(i).bl, CollisionBox.at(i).tl, CollisionBox.at(i).tr, axis);
			if (tmp != false) {
				return;
			}

			tmp = check_side_r_l(buffer, p, CollisionBox.at(collisionBox).tl, CollisionBox.at(collisionBox).bl, CollisionBox.at(i).tr, CollisionBox.at(i).br, CollisionBox.at(i).bl, axis);
			if (tmp != false) {
				return;
			}

			tmp = check_side_t_b(buffer, p, CollisionBox.at(collisionBox).br, CollisionBox.at(collisionBox).bl, CollisionBox.at(i).tr, CollisionBox.at(i).tl, CollisionBox.at(i).bl, axis);
			if (tmp != false) {
				if (p.Gravity != -1/* && axis == Y_AXIS*/) Gravity.at(p.Gravity).isGrounded = true;
				return;
			}

			tmp = check_side_b_t(buffer, p, CollisionBox.at(collisionBox).tl, CollisionBox.at(collisionBox).tr, CollisionBox.at(i).bl, CollisionBox.at(i).br, CollisionBox.at(i).tl, axis);
			if (tmp != false) {
				return;
			}
		}
	}
	if (p.Gravity != -1/* && axis == Y_AXIS*/) {
		Gravity.at(p.Gravity).collides = false;
		Gravity.at(p.Gravity).isGrounded = false;
	}
	return;
}

/*

for each non-static entity with collision box do {
	move entity on x-axis
	for each entity with collision box do {
		...
	}

	move entity on y-axis
	for each entity with collision box do {
		...
	}
}

*/
