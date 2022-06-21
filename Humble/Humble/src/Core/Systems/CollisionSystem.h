#pragma once
#include "../Header.h"
#include "../Utilities.h"
#include "RenderingSystem.h"
#include <vector>

namespace HBL {

	class CollisionSystem {
	public:
		void Start();
		void Run0(VertexBuffer& buffer);
		void Run(VertexBuffer& buffer);
		void Clear();

		bool CollisionBetween(IEntity& e0, IEntity& e1, VertexBuffer& buffer);
		std::vector<int> entities_to_be_updated;

	private:
		bool check_corner_br_tl_bool(glm::vec3 p_br, glm::vec3 e_tl, glm::vec3 e_br);
		bool check_corner_tr_bl_bool(glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_tr);
		bool check_corner_tl_br_bool(glm::vec3 p_tl, glm::vec3 e_br, glm::vec3 e_tl);
		bool check_corner_bl_tr_bool(glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_bl);

		void change_position_x(IEntity& p, VertexBuffer& buffer);
		void change_position_y(IEntity& p, VertexBuffer& buffer);

		bool check_corner_br_tl(VertexBuffer& buffer, IEntity& p, glm::vec3 p_br, glm::vec3 e_tl, glm::vec3 e_br, int axis);
		bool check_corner_tr_bl(VertexBuffer& buffer, IEntity& p, glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_tr, int axis);
		bool check_corner_tl_br(VertexBuffer& buffer, IEntity& p, glm::vec3 p_tl, glm::vec3 e_br, glm::vec3 e_tl, int axis);
		bool check_corner_bl_tr(VertexBuffer& buffer, IEntity& p, glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_bl, int axis);

		bool check_side_l_r(VertexBuffer& buffer, IEntity& p, glm::vec3 p_br, glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_tl, glm::vec3 e_tr, int axis);
		bool check_side_r_l(VertexBuffer& buffer, IEntity& p, glm::vec3 p_tl, glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_br, glm::vec3 e_bl, int axis);
		bool check_side_b_t(VertexBuffer& buffer, IEntity& p, glm::vec3 p_tl, glm::vec3 p_tr, glm::vec3 e_bl, glm::vec3 e_br, glm::vec3 e_tl, int axis);
		bool check_side_t_b(VertexBuffer& buffer, IEntity& p, glm::vec3 p_br, glm::vec3 p_bl, glm::vec3 e_tr, glm::vec3 e_tl, glm::vec3 e_bl, int axis);

		void Check_For_Collisions(IEntity& p, int collisionBox, VertexBuffer& buffer, int axis);
	};

}
