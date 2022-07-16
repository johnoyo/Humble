#include "CollisionSystem.h"
#include "../SystemsHeader.h"

namespace HBL {

	void CollisionSystem::Start()
	{
		FUNCTION_PROFILE();

		Filter(Globals::entities, "Transform", "CollisionBox");

		For_Each([&](IEntity& entt)
		{
			Component::Transform& transfom = GET_COMPONENT(Transform, entt);
			Component::CollisionBox& collisionBox = GET_COMPONENT(CollisionBox, entt);

			collisionBox.tl.x = transfom.position.x - transfom.scale.x / 2.0f;
			collisionBox.tl.y = transfom.position.y + transfom.scale.y / 2.0f;
													  
			collisionBox.tr.x = transfom.position.x + transfom.scale.x / 2.0f;
			collisionBox.tr.y = transfom.position.y + transfom.scale.y / 2.0f;
													  
			collisionBox.br.x = transfom.position.x + transfom.scale.x / 2.0f;
			collisionBox.br.y = transfom.position.y - transfom.scale.y / 2.0f;
													  
			collisionBox.bl.x = transfom.position.x - transfom.scale.x / 2.0f;
			collisionBox.bl.y = transfom.position.y - transfom.scale.y / 2.0f;
		});
	}

	void CollisionSystem::Run()
	{
		VertexBuffer& buffer = GlobalSystems::renderingSystem.Get_Vertex_Buffer();

		// Update collision boxes of non-static objects
		For_Each([&](IEntity& entt)
		{
			if (GET_COMPONENT(Transform, entt).Static == false) 
			{
				Component::Transform& transfom = GET_COMPONENT(Transform, entt);
				Component::CollisionBox& collisionBox = GET_COMPONENT(CollisionBox, entt);

				glm::vec3& tr = transfom.position;
				glm::vec3& sc = transfom.scale;

				collisionBox.tl.x = tr.x - sc.x / 2.0f;
				collisionBox.tr.x = tr.x + sc.x / 2.0f;
				collisionBox.br.x = tr.x + sc.x / 2.0f;
				collisionBox.bl.x = tr.x - sc.x / 2.0f;

				// collision check on x-axis
				Check_For_Collisions(entt, entt.components["CollisionBox"], buffer, X_AXIS);

				// update collision box on y-axis
				collisionBox.tl.y = tr.y + sc.y / 2.0f;
				collisionBox.tr.y = tr.y + sc.y / 2.0f;
				collisionBox.br.y = tr.y - sc.y / 2.0f;
				collisionBox.bl.y = tr.y - sc.y / 2.0f;

				// collision check on y-axis
				Check_For_Collisions(entt, entt.components["CollisionBox"], buffer, Y_AXIS);
			}
		});
	}

	void CollisionSystem::Clear()
	{
		Globals::CollisionBox.clear();
	}

	bool CollisionSystem::CollisionBetween(IEntity& e0, IEntity& e1)
	{
		VertexBuffer& buffer = GlobalSystems::renderingSystem.Get_Vertex_Buffer();

		bool collision = false;
		Component::CollisionBox& collisionBox0 = GET_COMPONENT(CollisionBox, e0);
		Component::CollisionBox& collisionBox1 = GET_COMPONENT(CollisionBox, e1);

		collision = check_corner_br_tl_bool(collisionBox0.br, collisionBox1.tl, collisionBox1.br);
		if (collision) return true;

		collision = check_corner_tr_bl_bool(collisionBox0.tr, collisionBox1.bl, collisionBox1.tr);
		if (collision) return true;

		collision = check_corner_tl_br_bool(collisionBox0.tl, collisionBox1.br, collisionBox1.tl);
		if (collision) return true;

		collision = check_corner_bl_tr_bool(collisionBox0.bl, collisionBox1.tr, collisionBox1.bl);
		if (collision) return true;

		return false;
	}

	bool CollisionSystem::check_corner_br_tl_bool(glm::vec3& p_br, glm::vec3& e_tl, glm::vec3& e_br) {
		if (p_br.y <= e_tl.y && p_br.x >= e_tl.x && p_br.y >= e_br.y && p_br.x <= e_br.x)
			return true;
		return false;
	}

	bool CollisionSystem::check_corner_tr_bl_bool(glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tr) {
		if (p_tr.y >= e_bl.y && p_tr.x >= e_bl.x && p_tr.y <= e_tr.y && p_tr.x <= e_tr.x)
			return true;
		return false;
	}

	bool CollisionSystem::check_corner_tl_br_bool(glm::vec3& p_tl, glm::vec3& e_br, glm::vec3& e_tl) {
		if (p_tl.y >= e_br.y && p_tl.x <= e_br.x && p_tl.y <= e_tl.y && p_tl.x >= e_tl.x)
			return true;
		return false;
	}

	bool CollisionSystem::check_corner_bl_tr_bool(glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_bl) {
		if (p_bl.y <= e_tr.y && p_bl.x <= e_tr.x && p_bl.y >= e_bl.y && p_bl.x >= e_bl.x)
			return true;
		return false;
	}

	void CollisionSystem::change_position_x(IEntity& p, VertexBuffer& buffer) 
	{
		Component::Transform& transfom = GET_COMPONENT(Transform, p);
		Component::CollisionBox& collisionBox = GET_COMPONENT(CollisionBox, p);

		glm::vec3 tr = transfom.position;
		glm::vec3 sc = transfom.scale;

		// update collision box on x-axis
		collisionBox.tl.x = tr.x - sc.x / 2.0f;
		collisionBox.tr.x = tr.x + sc.x / 2.0f;
		collisionBox.br.x = tr.x + sc.x / 2.0f;
		collisionBox.bl.x = tr.x - sc.x / 2.0f;

		// move player on x-axis
		buffer.Update_PositionX_On_Quad(transfom.bufferIndex, transfom);
	}

	void CollisionSystem::change_position_y(IEntity& p, VertexBuffer& buffer) 
	{
		Component::Transform& transfom = GET_COMPONENT(Transform, p);
		Component::CollisionBox& collisionBox = GET_COMPONENT(CollisionBox, p);

		glm::vec3 tr = transfom.position;
		glm::vec3 sc = transfom.scale;

		// update collision box on y-axis
		collisionBox.tl.y = tr.y + sc.y / 2.0f;
		collisionBox.tr.y = tr.y + sc.y / 2.0f;
		collisionBox.br.y = tr.y - sc.y / 2.0f;
		collisionBox.bl.y = tr.y - sc.y / 2.0f;

		// move player on y-axis
		buffer.Update_PositionY_On_Quad(transfom.bufferIndex, transfom);
	}

	bool CollisionSystem::check_corner_br_tl(VertexBuffer& buffer, IEntity& p, glm::vec3& p_br, glm::vec3& e_tl, glm::vec3& e_br, int axis)
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

	bool CollisionSystem::check_corner_tr_bl(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tr, int axis)
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

	bool CollisionSystem::check_corner_tl_br(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tl, glm::vec3& e_br, glm::vec3& e_tl, int axis)
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

	bool CollisionSystem::check_corner_bl_tr(VertexBuffer& buffer, IEntity& p, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_bl, int axis)
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

	bool CollisionSystem::check_side_l_r(VertexBuffer& buffer, IEntity& p, glm::vec3& p_br, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tl, glm::vec3& e_tr, int axis)
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

	bool CollisionSystem::check_side_r_l(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tl, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_br, glm::vec3& e_bl, int axis)
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

	bool CollisionSystem::check_side_b_t(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tl, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_br, glm::vec3& e_tl, int axis)
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

	bool CollisionSystem::check_side_t_b(VertexBuffer& buffer, IEntity& p, glm::vec3& p_br, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_tl, glm::vec3& e_bl, int axis)
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
		for (i = 0; i < Globals::CollisionBox.size(); i++) {
			bool tmp = false;
			if (i != collisionBox && Globals::CollisionBox.at(i).Enabled) {

				Component::CollisionBox& cb_p = Globals::CollisionBox.at(collisionBox);
				Component::Gravity& gr_p = GET_COMPONENT(Gravity, p);
				Component::CollisionBox& cb_i = Globals::CollisionBox.at(i);

				tmp = check_corner_br_tl(buffer, p, cb_p.br, cb_i.tl, cb_i.br, axis);
				if (tmp != false) {
					if (TRY_FIND_COMPONENT(Gravity, p)) gr_p.isGrounded = true;
					return;
				}

				tmp = check_corner_tr_bl(buffer, p, cb_p.tr, cb_i.bl, cb_i.tr, axis);
				if (tmp != false) {
					return;
				}

				tmp = check_corner_tl_br(buffer, p, cb_p.tl, cb_i.br, cb_i.tl, axis);
				if (tmp != false) {
					return;
				}

				tmp = check_corner_bl_tr(buffer, p, cb_p.bl, cb_i.tr, cb_i.bl, axis);
				if (tmp != false) {
					if (TRY_FIND_COMPONENT(Gravity, p)) gr_p.isGrounded = true;
					return;
				}

				tmp = check_side_l_r(buffer, p, cb_p.br, cb_p.tr, cb_i.bl, cb_i.tl, cb_i.tr, axis);
				if (tmp != false) {
					return;
				}

				tmp = check_side_r_l(buffer, p, cb_p.tl, cb_p.bl, cb_i.tr, cb_i.br, cb_i.bl, axis);
				if (tmp != false) {
					return;
				}

				tmp = check_side_t_b(buffer, p, cb_p.br, cb_p.bl, cb_i.tr, cb_i.tl, cb_i.bl, axis);
				if (tmp != false) {
					if (TRY_FIND_COMPONENT(Gravity, p)) gr_p.isGrounded = true;
					return;
				}

				tmp = check_side_b_t(buffer, p, cb_p.tl, cb_p.tr, cb_i.bl, cb_i.br, cb_i.tl, axis);
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
