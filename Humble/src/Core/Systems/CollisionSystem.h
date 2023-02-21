#pragma once

#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ECS/ISystem.h"
#include "../ECS/IEntity.h"
#include "../ECS/Registry.h"
#include "../ECS/Components.h"
#include "SpriteRendererSystem.h"

#include <list>
#include <vector>

namespace HBL 
{
	class HBL_API CollisionSystem final : public ISystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;

		void CreateSectors(glm::vec2& dimension, glm::vec2& worldSize);

		int Categorize(Component::Transform& transfom, Component::CollisionBox& collisionBox, const IEntity& entt);
		int FindSector(Component::Transform& transfom);

		bool CollisionBetween(IEntity& e0, IEntity& e1);
		bool CollisionBetween(Component::CollisionBox& collisionBox0, Component::CollisionBox& collisionBox1);

	private:
		std::vector<std::list<IEntity>> sectors;
		glm::vec2 sectorDimension = {};
		glm::vec2 sectorSize = {};

		bool CheckCornerBRTL(glm::vec3& p_br, glm::vec3& e_tl, glm::vec3& e_br);
		bool CheckCornerTRBL(glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tr);
		bool CheckCornerTLBR(glm::vec3& p_tl, glm::vec3& e_br, glm::vec3& e_tl);
		bool CheckCornerBLTR(glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_bl);

		bool CheckSideLR(glm::vec3& p_br, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tl, glm::vec3& e_tr);
		bool CheckSideRL(glm::vec3& p_tl, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_br, glm::vec3& e_bl);
		bool CheckSideBT(glm::vec3& p_tl, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_br, glm::vec3& e_tl);
		bool CheckSideTB(glm::vec3& p_br, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_tl, glm::vec3& e_bl);

		void ChangePositionX(IEntity& p, VertexBuffer& buffer);
		void ChangePositionY(IEntity& p, VertexBuffer& buffer);

		bool CheckCornerBRTL(VertexBuffer& buffer, IEntity& p, glm::vec3& p_br, glm::vec3& e_tl, glm::vec3& e_br, int axis);
		bool CheckCornerTRBL(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tr, int axis);
		bool CheckCornerTLBR(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tl, glm::vec3& e_br, glm::vec3& e_tl, int axis);
		bool CheckCornerBLTR(VertexBuffer& buffer, IEntity& p, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_bl, int axis);

		bool CheckSideLR(VertexBuffer& buffer, IEntity& p, glm::vec3& p_br, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tl, glm::vec3& e_tr, int axis);
		bool CheckSideRL(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tl, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_br, glm::vec3& e_bl, int axis);
		bool CheckSideBT(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tl, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_br, glm::vec3& e_tl, int axis);
		bool CheckSideTB(VertexBuffer& buffer, IEntity& p, glm::vec3& p_br, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_tl, glm::vec3& e_bl, int axis);

		bool CheckForCollisions(IEntity& p, VertexBuffer& buffer, int axis);
		bool CheckForSectorCollisions(IEntity& p, int index, VertexBuffer& buffer, int axis);
	};

}
