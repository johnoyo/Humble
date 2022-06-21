#include "CollisionSystem.h"

namespace HBL {

	void CollisionSystem::Start()
	{
		ENGINE_PROFILE("CollisionSystem::Start");

		for (uint32_t i = 0; i < entities.size(); i++) {
			if (TRY_FIND_COMPONENT(CollisionBox, entities.at(i)) && TRY_FIND_COMPONENT(Transform, entities.at(i))) {
				GET_COMPONENT(CollisionBox, entities.at(i)).tl.x = GET_COMPONENT(Transform, entities.at(i)).position.x - GET_COMPONENT(Transform, entities.at(i)).scale.x / 2.0f;
				GET_COMPONENT(CollisionBox, entities.at(i)).tl.y = GET_COMPONENT(Transform, entities.at(i)).position.y + GET_COMPONENT(Transform, entities.at(i)).scale.y / 2.0f;
																														 
				GET_COMPONENT(CollisionBox, entities.at(i)).tr.x = GET_COMPONENT(Transform, entities.at(i)).position.x + GET_COMPONENT(Transform, entities.at(i)).scale.x / 2.0f;
				GET_COMPONENT(CollisionBox, entities.at(i)).tr.y = GET_COMPONENT(Transform, entities.at(i)).position.y + GET_COMPONENT(Transform, entities.at(i)).scale.y / 2.0f;
																														 
				GET_COMPONENT(CollisionBox, entities.at(i)).br.x = GET_COMPONENT(Transform, entities.at(i)).position.x + GET_COMPONENT(Transform, entities.at(i)).scale.x / 2.0f;
				GET_COMPONENT(CollisionBox, entities.at(i)).br.y = GET_COMPONENT(Transform, entities.at(i)).position.y - GET_COMPONENT(Transform, entities.at(i)).scale.y / 2.0f;
																   														 
				GET_COMPONENT(CollisionBox, entities.at(i)).bl.x = GET_COMPONENT(Transform, entities.at(i)).position.x - GET_COMPONENT(Transform, entities.at(i)).scale.x / 2.0f;
				GET_COMPONENT(CollisionBox, entities.at(i)).bl.y = GET_COMPONENT(Transform, entities.at(i)).position.y - GET_COMPONENT(Transform, entities.at(i)).scale.y / 2.0f;
			}
		}
	}

	void CollisionSystem::Run0(VertexBuffer& buffer)
	{
		//ENGINE_PROFILE("CollisionSystem::Run0");

		// update collision boxes and move the Transform of non-static objects
		for (uint32_t i = 0; i < entities.size(); i++) {
			if (TRY_FIND_COMPONENT(Transform, entities.at(i)) && GET_COMPONENT(Transform, entities.at(i)).Static == false) {
				IEntity& entt = entities.at(i);

				glm::vec3 tr = GET_COMPONENT(Transform, entt).position;
				glm::vec3 sc = GET_COMPONENT(Transform, entt).scale;

				// move player on x-axis
				buffer.Update_PositionX_On_Quad(GET_COMPONENT(Transform, entt).bufferIndex, GET_COMPONENT(Transform, entt));

				// update collision box on x-axis
				if (TRY_FIND_COMPONENT(CollisionBox, entities.at(i))) {
					GET_COMPONENT(CollisionBox, entt).tl.x = tr.x - sc.x / 2.0f;
					GET_COMPONENT(CollisionBox, entt).tr.x = tr.x + sc.x / 2.0f;
					GET_COMPONENT(CollisionBox, entt).br.x = tr.x + sc.x / 2.0f;
					GET_COMPONENT(CollisionBox, entt).bl.x = tr.x - sc.x / 2.0f;
				}

				// collision check on x-axis
				if (TRY_FIND_COMPONENT(CollisionBox, entt)) Check_For_Collisions(entt, entt.CollisionBox, buffer, X_AXIS);

				// move player on y-axis
				buffer.Update_PositionY_On_Quad(GET_COMPONENT(Transform, entt).bufferIndex, GET_COMPONENT(Transform, entt));

				// update collision box on y-axis
				if (TRY_FIND_COMPONENT(CollisionBox, entt)) {
					GET_COMPONENT(CollisionBox, entt).tl.y = tr.y + sc.y / 2.0f;
					GET_COMPONENT(CollisionBox, entt).tr.y = tr.y + sc.y / 2.0f;
					GET_COMPONENT(CollisionBox, entt).br.y = tr.y - sc.y / 2.0f;
					GET_COMPONENT(CollisionBox, entt).bl.y = tr.y - sc.y / 2.0f;
				}

				// collision check on y-axis
				if (TRY_FIND_COMPONENT(CollisionBox, entt)) Check_For_Collisions(entt, entt.CollisionBox, buffer, Y_AXIS);
			}
		}
	}

	void CollisionSystem::Run(VertexBuffer& buffer)
	{
		//ENGINE_PROFILE("CollisionSystem::Run");

		// update collision boxes of non-static objects
		for (uint32_t i = 0; i < entities.size(); i++) {
			if (TRY_FIND_COMPONENT(Transform, entities.at(i)) && GET_COMPONENT(Transform, entities.at(i)).Static == false) {
				IEntity& entt = entities.at(i);

				glm::vec3& tr = GET_COMPONENT(Transform, entt).position;
				glm::vec3& sc = GET_COMPONENT(Transform, entt).scale;

				// update collision box on x-axis
				if (TRY_FIND_COMPONENT(CollisionBox, entities.at(i))) {
					GET_COMPONENT(CollisionBox, entt).tl.x = tr.x - sc.x / 2.0f;
					GET_COMPONENT(CollisionBox, entt).tr.x = tr.x + sc.x / 2.0f;
					GET_COMPONENT(CollisionBox, entt).br.x = tr.x + sc.x / 2.0f;
					GET_COMPONENT(CollisionBox, entt).bl.x = tr.x - sc.x / 2.0f;
				}

				// collision check on x-axis
				if (TRY_FIND_COMPONENT(CollisionBox, entities.at(i))) Check_For_Collisions(entt, entt.CollisionBox, buffer, X_AXIS);

				// update collision box on y-axis
				if (TRY_FIND_COMPONENT(CollisionBox, entities.at(i))) {
					GET_COMPONENT(CollisionBox, entt).tl.y = tr.y + sc.y / 2.0f;
					GET_COMPONENT(CollisionBox, entt).tr.y = tr.y + sc.y / 2.0f;
					GET_COMPONENT(CollisionBox, entt).br.y = tr.y - sc.y / 2.0f;
					GET_COMPONENT(CollisionBox, entt).bl.y = tr.y - sc.y / 2.0f;
				}

				// collision check on y-axis
				if (TRY_FIND_COMPONENT(CollisionBox, entities.at(i))) Check_For_Collisions(entt, entt.CollisionBox, buffer, Y_AXIS);

			}
		}
	}

	void CollisionSystem::Clear()
	{
		CollisionBox.clear();
	}

	bool CollisionSystem::CollisionBetween(IEntity& e0, IEntity& e1, VertexBuffer& buffer)
	{
		bool collision = false;

		collision = check_corner_br_tl_bool(GET_COMPONENT(CollisionBox, e0).br, GET_COMPONENT(CollisionBox, e1).tl, GET_COMPONENT(CollisionBox, e1).br);
		if (collision) return true;

		collision = check_corner_tr_bl_bool(GET_COMPONENT(CollisionBox, e0).tr, GET_COMPONENT(CollisionBox, e1).bl, GET_COMPONENT(CollisionBox, e1).tr);
		if (collision) return true;

		collision = check_corner_tl_br_bool(GET_COMPONENT(CollisionBox, e0).tl, GET_COMPONENT(CollisionBox, e1).br, GET_COMPONENT(CollisionBox, e1).tl);
		if (collision) return true;

		collision = check_corner_bl_tr_bool(GET_COMPONENT(CollisionBox, e0).bl, GET_COMPONENT(CollisionBox, e1).tr, GET_COMPONENT(CollisionBox, e1).bl);
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

	void CollisionSystem::change_position_x(IEntity& p, VertexBuffer& buffer) {

		glm::vec3 tr = GET_COMPONENT(Transform, p).position;
		glm::vec3 sc = GET_COMPONENT(Transform, p).scale;

		// update collision box on x-axis
		GET_COMPONENT(CollisionBox, p).tl.x = tr.x - sc.x / 2.0f;
		GET_COMPONENT(CollisionBox, p).tr.x = tr.x + sc.x / 2.0f;
		GET_COMPONENT(CollisionBox, p).br.x = tr.x + sc.x / 2.0f;
		GET_COMPONENT(CollisionBox, p).bl.x = tr.x - sc.x / 2.0f;

		// move player on x-axis
		entities_to_be_updated.push_back(p.Transform);
		buffer.Update_PositionX_On_Quad(GET_COMPONENT(Transform, p).bufferIndex, GET_COMPONENT(Transform, p));
	}

	void CollisionSystem::change_position_y(IEntity& p, VertexBuffer& buffer) {

		glm::vec3 tr = GET_COMPONENT(Transform, p).position;
		glm::vec3 sc = GET_COMPONENT(Transform, p).scale;

		// update collision box on y-axis
		GET_COMPONENT(CollisionBox, p).tl.y = tr.y + sc.y / 2.0f;
		GET_COMPONENT(CollisionBox, p).tr.y = tr.y + sc.y / 2.0f;
		GET_COMPONENT(CollisionBox, p).br.y = tr.y - sc.y / 2.0f;
		GET_COMPONENT(CollisionBox, p).bl.y = tr.y - sc.y / 2.0f;

		// move player on y-axis
		entities_to_be_updated.push_back(p.Transform);
		buffer.Update_PositionY_On_Quad(GET_COMPONENT(Transform, p).bufferIndex, GET_COMPONENT(Transform, p));
	}

	bool CollisionSystem::check_corner_br_tl(VertexBuffer& buffer, IEntity& p, glm::vec3 p_br, glm::vec3 e_tl, glm::vec3 e_br, int axis)
	{
		if (p_br.y < e_tl.y && p_br.x > e_tl.x && p_br.y > e_br.y && p_br.x < e_br.x) {
			//std::cout << "bottom right\n";
			if (axis == X_AXIS) {
				GET_COMPONENT(Transform, p).position.x += e_tl.x - p_br.x;
				change_position_x(p, buffer);
			}
			else if (axis == Y_AXIS) {
				GET_COMPONENT(Transform, p).position.y += e_tl.y - p_br.y;
				change_position_y(p, buffer);
			}
			return true;
		}
		return false;
	}

	bool CollisionSystem::check_corner_tr_bl(VertexBuffer& buffer, IEntity& p, glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_tr, int axis)
	{
		if (p_tr.y > e_bl.y && p_tr.x > e_bl.x && p_tr.y < e_tr.y && p_tr.x < e_tr.x) {
			//std::cout << "top right\n";
			if (axis == X_AXIS) {
				GET_COMPONENT(Transform, p).position.x += e_bl.x - p_tr.x;
				change_position_x(p, buffer);
			}
			else if (axis == Y_AXIS) {
				//Transform.at(p.Transform).position.y += e_bl.y - 0.5f - p_tr.y;
				GET_COMPONENT(Transform, p).position.y += e_bl.y - p_tr.y;
				change_position_y(p, buffer);
			}
			return true;
		}
		return false;
	}

	bool CollisionSystem::check_corner_tl_br(VertexBuffer& buffer, IEntity& p, glm::vec3 p_tl, glm::vec3 e_br, glm::vec3 e_tl, int axis)
	{
		if (p_tl.y > e_br.y && p_tl.x < e_br.x && p_tl.y < e_tl.y && p_tl.x > e_tl.x) {
			//std::cout << "top left\n";
			if (axis == X_AXIS) {
				GET_COMPONENT(Transform, p).position.x += e_br.x - p_tl.x;
				change_position_x(p, buffer);
			}
			else if (axis == Y_AXIS) {
				//Transform.at(p.Transform).position.y += e_br.y - 0.5f - p_tl.y;
				GET_COMPONENT(Transform, p).position.y += e_br.y - p_tl.y;
				change_position_y(p, buffer);
			}
			return true;
		}
		return false;
	}

	bool CollisionSystem::check_corner_bl_tr(VertexBuffer& buffer, IEntity& p, glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_bl, int axis)
	{
		if (p_bl.y < e_tr.y && p_bl.x < e_tr.x && p_bl.y > e_bl.y && p_bl.x > e_bl.x) {
			//std::cout << "bottom left\n";
			if (axis == X_AXIS) {
				GET_COMPONENT(Transform, p).position.x += e_tr.x - p_bl.x;
				change_position_x(p, buffer);
			}
			else if (axis == Y_AXIS) {
				GET_COMPONENT(Transform, p).position.y += e_tr.y - p_bl.y;
				change_position_y(p, buffer);
			}
			return true;
		}
		return false;
	}

	bool CollisionSystem::check_side_l_r(VertexBuffer& buffer, IEntity& p, glm::vec3 p_br, glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_tl, glm::vec3 e_tr, int axis)
	{
		if (p_br.y <= e_bl.y && p_tr.y >= e_tl.y && p_br.x > e_bl.x && p_tr.x > e_tl.x && p_tr.x < e_tr.x) {
			if (axis == X_AXIS) {
				//std::cout << "left to right\n";
				GET_COMPONENT(Transform, p).position.x += e_bl.x - p_br.x;
				change_position_x(p, buffer);
			}

			return true;
		}
		return false;
	}

	bool CollisionSystem::check_side_r_l(VertexBuffer& buffer, IEntity& p, glm::vec3 p_tl, glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_br, glm::vec3 e_bl, int axis)
	{
		if (p_tl.y >= e_tr.y && p_bl.y <= e_br.y && p_tl.x < e_tr.x && p_bl.x < e_br.x && p_tl.x > e_bl.x) {
			if (axis == X_AXIS) {
				//std::cout << "right to left\n";
				GET_COMPONENT(Transform, p).position.x += e_br.x - p_bl.x;
				change_position_x(p, buffer);
			}

			return true;
		}
		return false;
	}

	bool CollisionSystem::check_side_b_t(VertexBuffer& buffer, IEntity& p, glm::vec3 p_tl, glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_br, glm::vec3 e_tl, int axis)
	{
		if (p_tl.x <= e_bl.x && p_tl.y > e_bl.y && p_tr.x >= e_br.x && p_tr.y > e_br.y && p_tl.y < e_tl.y) {
			if (axis == Y_AXIS) {
				//std::cout << "bottom to top\n";
				//Transform.at(p.Transform).position.y += e_bl.y - 0.5f - p_tl.y;
				GET_COMPONENT(Transform, p).position.y += e_bl.y - p_tl.y;
				change_position_y(p, buffer);
			}

			return true;
		}
		return false;
	}

	bool CollisionSystem::check_side_t_b(VertexBuffer& buffer, IEntity& p, glm::vec3 p_br, glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_tl, glm::vec3 e_bl, int axis)
	{
		if (p_bl.x <= e_tl.x && p_bl.y < e_tl.y && p_br.y < e_tr.y && p_br.x >= e_tr.x && p_bl.y > e_bl.y) {
			if (axis == Y_AXIS) {
				//std::cout << "top to bottom\n";
				GET_COMPONENT(Transform, p).position.y += e_tl.y - p_bl.y;
				change_position_y(p, buffer);
			}

			return true;
		}
		return false;
	}

	void CollisionSystem::Check_For_Collisions(IEntity& p, int collisionBox, VertexBuffer& buffer, int axis)
	{
		uint32_t i = 0;
		if (TRY_FIND_COMPONENT(Gravity, p)) {
			GET_COMPONENT(Gravity, p).collides = true;
			GET_COMPONENT(Gravity, p).isGrounded = false;
		}
		for (i = 0; i < CollisionBox.size(); i++) {
			bool tmp = false;
			if (i != collisionBox && CollisionBox.at(i).Enabled) {

				tmp = check_corner_br_tl(buffer, p, CollisionBox.at(collisionBox).br, CollisionBox.at(i).tl, CollisionBox.at(i).br, axis);
				if (tmp != false) {
					if (TRY_FIND_COMPONENT(Gravity, p)) GET_COMPONENT(Gravity, p).isGrounded = true;
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
					if (TRY_FIND_COMPONENT(Gravity, p)) GET_COMPONENT(Gravity, p).isGrounded = true;
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
					if (TRY_FIND_COMPONENT(Gravity, p)) GET_COMPONENT(Gravity, p).isGrounded = true;
					return;
				}

				tmp = check_side_b_t(buffer, p, CollisionBox.at(collisionBox).tl, CollisionBox.at(collisionBox).tr, CollisionBox.at(i).bl, CollisionBox.at(i).br, CollisionBox.at(i).tl, axis);
				if (tmp != false) {
					return;
				}
			}
		}
		if (TRY_FIND_COMPONENT(Gravity, p)) {
			GET_COMPONENT(Gravity, p).collides = false;
			GET_COMPONENT(Gravity, p).isGrounded = false;
		}
		return;
	}

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
