#include "CollisionSystem.h"

namespace HBL 
{
	void CollisionSystem::Start()
	{
		FUNCTION_PROFILE();

		CreateSectors(SceneManager::Get().m_SectorSize, SceneManager::Get().m_WorldSize);

		Registry::Get().Group<Component::Transform, Component::CollisionBox>().ForEach([&](IEntity& entt)
		{
			Component::Transform& transfom = Registry::Get().GetComponent<Component::Transform>(entt);
			Component::CollisionBox& collisionBox = Registry::Get().GetComponent<Component::CollisionBox>(entt);

			collisionBox.tl.x = transfom.position.x - transfom.scale.x / 2.0f;
			collisionBox.tl.y = transfom.position.y + transfom.scale.y / 2.0f;
													  
			collisionBox.tr.x = transfom.position.x + transfom.scale.x / 2.0f;
			collisionBox.tr.y = transfom.position.y + transfom.scale.y / 2.0f;
													  
			collisionBox.br.x = transfom.position.x + transfom.scale.x / 2.0f;
			collisionBox.br.y = transfom.position.y - transfom.scale.y / 2.0f;
													  
			collisionBox.bl.x = transfom.position.x - transfom.scale.x / 2.0f;
			collisionBox.bl.y = transfom.position.y - transfom.scale.y / 2.0f;

			Categorize(transfom, collisionBox, entt);
		}).Run();

	}

	void CollisionSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		// Update collision boxes of non-static objects
		Registry::Get().Group<Component::Transform, Component::CollisionBox>().ForEach([&](IEntity& entt)
		{
			Component::CollisionBox& collisionBox = Registry::Get().GetComponent<Component::CollisionBox>(entt);
			
			if (collisionBox.Enabled)
			{
				Component::Transform& transfom = Registry::Get().GetComponent<Component::Transform>(entt);

				if (transfom.Static == false)
				{
					glm::vec3& tr = transfom.position;
					glm::vec3& sc = transfom.scale;

					// Update sector category.
					int index = Categorize(transfom, collisionBox, entt);

					bool xCollision = false;
					bool yCollision = false;

					// Update collision box on x-axis.
					collisionBox.tl.x = tr.x - sc.x / 2.0f;
					collisionBox.tr.x = tr.x + sc.x / 2.0f;
					collisionBox.br.x = tr.x + sc.x / 2.0f;
					collisionBox.bl.x = tr.x - sc.x / 2.0f;

					// Collision check on x-axis.
					if (index != -1)
						xCollision = CheckForSectorCollisions(entt, index, buffer, X_AXIS);
					else
						xCollision = CheckForCollisions(entt, buffer, X_AXIS);

					// Update collision box on y-axis.
					collisionBox.tl.y = tr.y + sc.y / 2.0f;
					collisionBox.tr.y = tr.y + sc.y / 2.0f;
					collisionBox.br.y = tr.y - sc.y / 2.0f;
					collisionBox.bl.y = tr.y - sc.y / 2.0f;

					// Collision check on y-axis.
					if (index != -1)
						yCollision = CheckForSectorCollisions(entt, index, buffer, Y_AXIS);
					else
						yCollision = CheckForCollisions(entt, buffer, Y_AXIS);

					if (Registry::Get().HasComponent<Component::Gravity>(entt))
					{
						if (!xCollision && !yCollision)
						{
							Registry::Get().GetComponent<Component::Gravity>(entt).collides = false;
							Registry::Get().GetComponent<Component::Gravity>(entt).isGrounded = false;
						}
					}
				}
			}
		}).Run();
	}

	void CollisionSystem::Clear()
	{
		Registry::Get().ClearArray<Component::CollisionBox>();

		for (uint32_t i = 0; i < sectors.size(); i++)
		{
			sectors[i].clear();
		}

		sectors.clear();
	}

	void CollisionSystem::CreateSectors(glm::vec2& dimension, glm::vec2& worldSize)
	{
		sectorDimension = dimension;
		sectorSize = { glm::ceil((worldSize.x * SceneManager::Get().m_TileSize) / sectorDimension[0]), glm::ceil((worldSize.y * SceneManager::Get().m_TileSize) / sectorDimension[1]) };

		for (uint32_t i = 0; i < (sectorDimension[0] * sectorDimension[1]); i++)
		{
			sectors.push_back(std::list<IEntity>());
		}

		return;
	}

	int CollisionSystem::Categorize(Component::Transform& transfom, Component::CollisionBox& collisionBox, const IEntity& entt)
	{
		//FUNCTION_PROFILE();

		uint32_t cachedIndex = -1;

		if (transfom.Enabled)
		{
			// Remove entity from all sectors.
			for (uint32_t k = 0; k < sectorDimension[0] * sectorDimension[1]; k++)
				sectors[k].remove(entt);

			for (uint32_t i = 0; i < sectorDimension[1]; i++)
			{
				for (uint32_t j = 0; j < sectorDimension[0]; j++)
				{
					int index = (i * sectorDimension[1]) + j;

					if (transfom.position.x <= (sectorSize.x * j) + sectorSize.x && transfom.position.x >= (sectorSize.x * j) 
					 && transfom.position.y <= (sectorSize.y * i) + sectorSize.y && transfom.position.y >= (sectorSize.y * i))
					{
						// Add entity to sector.
						sectors[index].emplace_back(entt);

						// Cache index.
						cachedIndex = index;
					}
				}
			}
		}

		return cachedIndex;
	}

	int CollisionSystem::FindSector(Component::Transform& transfom)
	{
		//FUNCTION_PROFILE();

		if (transfom.Enabled)
		{
			for (uint32_t i = 0; i < sectorDimension[0]; i++)
			{
				for (uint32_t j = 0; j < sectorDimension[1]; j++)
				{
					int index = (i * sectorDimension[0]) + j;

					if (transfom.position.x <= (sectorSize.x * j) + sectorSize.x && transfom.position.x >= (sectorSize.x * j)
						&& transfom.position.y <= (sectorSize.y * i) + sectorSize.y && transfom.position.y >= (sectorSize.y * i))
					{
						return index;
					}
				}
			}
		}

		return -1;
	}

	bool CollisionSystem::CollisionBetween(IEntity& e0, IEntity& e1)
	{
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		bool collision = false;
		Component::CollisionBox& collisionBox0 = Registry::Get().GetComponent<Component::CollisionBox>(e0);
		Component::CollisionBox& collisionBox1 = Registry::Get().GetComponent<Component::CollisionBox>(e1);

		collision = CheckCornerBRTL(collisionBox0.br, collisionBox1.tl, collisionBox1.br);
		if (collision) return true;

		collision = CheckCornerTRBL(collisionBox0.tr, collisionBox1.bl, collisionBox1.tr);
		if (collision) return true;

		collision = CheckCornerTLBR(collisionBox0.tl, collisionBox1.br, collisionBox1.tl);
		if (collision) return true;

		collision = CheckCornerBLTR(collisionBox0.bl, collisionBox1.tr, collisionBox1.bl);
		if (collision) return true;

		collision = CheckSideLR(collisionBox0.br, collisionBox0.tr, collisionBox1.bl, collisionBox1.tl, collisionBox1.tr);
		if (collision) return true;

		collision = CheckSideLR(collisionBox0.tl, collisionBox0.bl, collisionBox1.tr, collisionBox1.br, collisionBox1.bl);
		if (collision) return true;

		collision = CheckSideLR(collisionBox0.tl, collisionBox0.tr, collisionBox1.bl, collisionBox1.br, collisionBox1.tl);
		if (collision) return true;

		collision = CheckSideLR(collisionBox0.br, collisionBox0.bl, collisionBox1.tr, collisionBox1.tl, collisionBox1.bl);
		if (collision) return true;

		return false;
	}

	bool CollisionSystem::CheckCornerBRTL(glm::vec3& p_br, glm::vec3& e_tl, glm::vec3& e_br) {
		if (p_br.y <= e_tl.y && p_br.x >= e_tl.x && p_br.y >= e_br.y && p_br.x <= e_br.x)
			return true;
		return false;
	}

	bool CollisionSystem::CheckCornerTRBL(glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tr) {
		if (p_tr.y >= e_bl.y && p_tr.x >= e_bl.x && p_tr.y <= e_tr.y && p_tr.x <= e_tr.x)
			return true;
		return false;
	}

	bool CollisionSystem::CheckCornerTLBR(glm::vec3& p_tl, glm::vec3& e_br, glm::vec3& e_tl) {
		if (p_tl.y >= e_br.y && p_tl.x <= e_br.x && p_tl.y <= e_tl.y && p_tl.x >= e_tl.x)
			return true;
		return false;
	}

	bool CollisionSystem::CheckCornerBLTR(glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_bl) {
		if (p_bl.y <= e_tr.y && p_bl.x <= e_tr.x && p_bl.y >= e_bl.y && p_bl.x >= e_bl.x)
			return true;
		return false;
	}

	bool CollisionSystem::CheckSideLR(glm::vec3& p_br, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tl, glm::vec3& e_tr)
	{
		if (p_br.y <= e_bl.y && p_tr.y >= e_tl.y && p_br.x >= e_bl.x && p_tr.x >= e_tl.x && p_tr.x <= e_tr.x)
			return true;
		return false;
	}

	bool CollisionSystem::CheckSideRL(glm::vec3& p_tl, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_br, glm::vec3& e_bl)
	{
		if (p_tl.y >= e_tr.y && p_bl.y <= e_br.y && p_tl.x <= e_tr.x && p_bl.x <= e_br.x && p_tl.x >= e_bl.x)
			return true;
		return false;
	}

	bool CollisionSystem::CheckSideBT(glm::vec3& p_tl, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_br, glm::vec3& e_tl)
	{
		if (p_tl.x <= e_bl.x && p_tl.y >= e_bl.y && p_tr.x >= e_br.x && p_tr.y >= e_br.y && p_tl.y <=e_tl.y)
			return true;
		return false;
	}

	bool CollisionSystem::CheckSideTB(glm::vec3& p_br, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_tl, glm::vec3& e_bl)
	{
		if (p_bl.x <= e_tl.x && p_bl.y <= e_tl.y && p_br.y <= e_tr.y && p_br.x >= e_tr.x && p_bl.y >= e_bl.y)
			return true;
		return false;
	}

	void CollisionSystem::ChangePositionX(IEntity& p, VertexBuffer& buffer) 
	{
		Component::Transform& transfom = Registry::Get().GetComponent<Component::Transform>(p);
		Component::CollisionBox& collisionBox = Registry::Get().GetComponent<Component::CollisionBox>(p);

		glm::vec3 tr = transfom.position;
		glm::vec3 sc = transfom.scale;

		// update collision box on x-axis
		collisionBox.tl.x = tr.x - sc.x / 2.0f;
		collisionBox.tr.x = tr.x + sc.x / 2.0f;
		collisionBox.br.x = tr.x + sc.x / 2.0f;
		collisionBox.bl.x = tr.x - sc.x / 2.0f;

		// move player on x-axis
		buffer.UpdatePositionXOnQuad(transfom.bufferIndex, transfom.position, transfom.scale);
	}

	void CollisionSystem::ChangePositionY(IEntity& p, VertexBuffer& buffer) 
	{
		Component::Transform& transfom = Registry::Get().GetComponent<Component::Transform>(p);
		Component::CollisionBox& collisionBox = Registry::Get().GetComponent<Component::CollisionBox>(p);

		glm::vec3 tr = transfom.position;
		glm::vec3 sc = transfom.scale;

		// update collision box on y-axis
		collisionBox.tl.y = tr.y + sc.y / 2.0f;
		collisionBox.tr.y = tr.y + sc.y / 2.0f;
		collisionBox.br.y = tr.y - sc.y / 2.0f;
		collisionBox.bl.y = tr.y - sc.y / 2.0f;

		// move player on y-axis
		buffer.UpdatePositionYOnQuad(transfom.bufferIndex, transfom.position, transfom.scale);
	}

	bool CollisionSystem::CheckCornerBRTL(VertexBuffer& buffer, IEntity& p, glm::vec3& p_br, glm::vec3& e_tl, glm::vec3& e_br, int axis)
	{
		if (p_br.y < e_tl.y && p_br.x > e_tl.x && p_br.y > e_br.y && p_br.x < e_br.x) 
		{
			//ENGINE_LOG("bottom right");
			if (axis == X_AXIS) {
				Registry::Get().GetComponent<Component::Transform>(p).position.x += e_tl.x - p_br.x;
				ChangePositionX(p, buffer);
			}
			else if (axis == Y_AXIS) {
				Registry::Get().GetComponent<Component::Transform>(p).position.y += e_tl.y - p_br.y;
				ChangePositionY(p, buffer);
			}
			return true;
		}
		return false;
	}

	bool CollisionSystem::CheckCornerTRBL(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tr, int axis)
	{
		if (p_tr.y > e_bl.y && p_tr.x > e_bl.x && p_tr.y < e_tr.y && p_tr.x < e_tr.x) 
		{
			//ENGINE_LOG("top right");
			if (axis == X_AXIS) 
			{
				Registry::Get().GetComponent<Component::Transform>(p).position.x += e_bl.x - p_tr.x;
				ChangePositionX(p, buffer);
			}
			else if (axis == Y_AXIS) 
			{
				//Transform.at(p.Transform).position.y += e_bl.y - 0.5f - p_tr.y;
				Registry::Get().GetComponent<Component::Transform>(p).position.y += e_bl.y - p_tr.y;
				ChangePositionY(p, buffer);
			}
			return true;
		}
		return false;
	}

	bool CollisionSystem::CheckCornerTLBR(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tl, glm::vec3& e_br, glm::vec3& e_tl, int axis)
	{
		if (p_tl.y > e_br.y && p_tl.x < e_br.x && p_tl.y < e_tl.y && p_tl.x > e_tl.x) 
		{
			//ENGINE_LOG("top left");
			if (axis == X_AXIS) 
			{
				Registry::Get().GetComponent<Component::Transform>(p).position.x += e_br.x - p_tl.x;
				ChangePositionX(p, buffer);
			}
			else if (axis == Y_AXIS) 
			{
				Registry::Get().GetComponent<Component::Transform>(p).position.y += e_br.y - p_tl.y;
				ChangePositionY(p, buffer);
			}
			return true;
		}
		return false;
	}

	bool CollisionSystem::CheckCornerBLTR(VertexBuffer& buffer, IEntity& p, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_bl, int axis)
	{
		if (p_bl.y < e_tr.y && p_bl.x < e_tr.x && p_bl.y > e_bl.y && p_bl.x > e_bl.x) 
		{
			//ENGINE_LOG("bottom left");
			if (axis == X_AXIS) 
			{
				Registry::Get().GetComponent<Component::Transform>(p).position.x += e_tr.x - p_bl.x;
				ChangePositionX(p, buffer);
			}
			else if (axis == Y_AXIS) 
			{
				Registry::Get().GetComponent<Component::Transform>(p).position.y += e_tr.y - p_bl.y;
				ChangePositionY(p, buffer);
			}
			return true;
		}
		return false;
	}

	bool CollisionSystem::CheckSideLR(VertexBuffer& buffer, IEntity& p, glm::vec3& p_br, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_tl, glm::vec3& e_tr, int axis)
	{
		if (p_br.y <= e_bl.y && p_tr.y >= e_tl.y && p_br.x > e_bl.x && p_tr.x > e_tl.x && p_tr.x < e_tr.x) 
		{
			if (axis == X_AXIS) 
			{
				//ENGINE_LOG("left to right");
				Registry::Get().GetComponent<Component::Transform>(p).position.x += e_bl.x - p_br.x;
				ChangePositionX(p, buffer);
			}

			return true;
		}
		return false;
	}

	bool CollisionSystem::CheckSideRL(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tl, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_br, glm::vec3& e_bl, int axis)
	{
		if (p_tl.y >= e_tr.y && p_bl.y <= e_br.y && p_tl.x < e_tr.x && p_bl.x < e_br.x && p_tl.x > e_bl.x) 
		{
			if (axis == X_AXIS) 
			{
				//ENGINE_LOG("right to left");
				Registry::Get().GetComponent<Component::Transform>(p).position.x += e_br.x - p_bl.x;
				ChangePositionX(p, buffer);
			}

			return true;
		}
		return false;
	}

	bool CollisionSystem::CheckSideBT(VertexBuffer& buffer, IEntity& p, glm::vec3& p_tl, glm::vec3& p_tr, glm::vec3& e_bl, glm::vec3& e_br, glm::vec3& e_tl, int axis)
	{
		if (p_tl.x <= e_bl.x && p_tl.y > e_bl.y && p_tr.x >= e_br.x && p_tr.y > e_br.y && p_tl.y < e_tl.y) 
		{
			if (axis == Y_AXIS) 
			{
				//ENGINE_LOG("bottom to top");
				Registry::Get().GetComponent<Component::Transform>(p).position.y += e_bl.y - p_tl.y;
				ChangePositionY(p, buffer);
			}

			return true;
		}
		return false;
	}

	bool CollisionSystem::CheckSideTB(VertexBuffer& buffer, IEntity& p, glm::vec3& p_br, glm::vec3& p_bl, glm::vec3& e_tr, glm::vec3& e_tl, glm::vec3& e_bl, int axis)
	{
		if (p_bl.x <= e_tl.x && p_bl.y < e_tl.y && p_br.y < e_tr.y && p_br.x >= e_tr.x && p_bl.y > e_bl.y) 
		{
			if (axis == Y_AXIS) 
			{
				//ENGINE_LOG("top to bottom");
				Registry::Get().GetComponent<Component::Transform>(p).position.y += e_tl.y - p_bl.y;
				ChangePositionY(p, buffer);
			}

			return true;
		}
		return false;
	}

	bool CollisionSystem::CheckForCollisions(IEntity& p, VertexBuffer& buffer, int axis)
	{
		//FUNCTION_PROFILE();

		uint32_t i = 0;

		for (auto& component : Registry::Get().GetArray<Component::CollisionBox>())
		{
			bool collision = false;
			Component::CollisionBox& cb_i = component.second;

			if (component.first != p.uuid && cb_i.Enabled)
			{
				Component::CollisionBox& cb_p = Registry::Get().GetComponent<Component::CollisionBox>(p);

				collision = CheckCornerBRTL(buffer, p, cb_p.br, cb_i.tl, cb_i.br, axis);
				if (collision) {
					if (Registry::Get().HasComponent<Component::Gravity>(p) && axis == Y_AXIS)
					{
						Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
						Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = true;
					}
					return true;
				}

				collision = CheckCornerTRBL(buffer, p, cb_p.tr, cb_i.bl, cb_i.tr, axis);
				if (collision) {
					if (Registry::Get().HasComponent<Component::Gravity>(p) && axis == Y_AXIS)
					{
						Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
						Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = false;
					}
					return true;
				}

				collision = CheckCornerTLBR(buffer, p, cb_p.tl, cb_i.br, cb_i.tl, axis);
				if (collision) {
					if (Registry::Get().HasComponent<Component::Gravity>(p) && axis == Y_AXIS)
					{
						Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
						Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = false;
					}
					return true;
				}

				collision = CheckCornerBLTR(buffer, p, cb_p.bl, cb_i.tr, cb_i.bl, axis);
				if (collision) {
					if (Registry::Get().HasComponent<Component::Gravity>(p) && axis == Y_AXIS)
					{
						Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
						Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = true;
					}
					return true;
				}

				collision = CheckSideLR(buffer, p, cb_p.br, cb_p.tr, cb_i.bl, cb_i.tl, cb_i.tr, axis);
				if (collision) {
					if (Registry::Get().HasComponent<Component::Gravity>(p) && axis == Y_AXIS)
					{
						Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
						Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = false;
					}
					return true;
				}

				collision = CheckSideRL(buffer, p, cb_p.tl, cb_p.bl, cb_i.tr, cb_i.br, cb_i.bl, axis);
				if (collision) {
					if (Registry::Get().HasComponent<Component::Gravity>(p) && axis == Y_AXIS)
					{
						Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
						Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = true;
					}
					return true;
				}

				collision = CheckSideTB(buffer, p, cb_p.br, cb_p.bl, cb_i.tr, cb_i.tl, cb_i.bl, axis);
				if (collision) {
					if (Registry::Get().HasComponent<Component::Gravity>(p) && axis == Y_AXIS)
					{
						Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
						Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = true;
					}
					return true;
				}

				collision = CheckSideBT(buffer, p, cb_p.tl, cb_p.tr, cb_i.bl, cb_i.br, cb_i.tl, axis);
				if (collision) {
					if (Registry::Get().HasComponent<Component::Gravity>(p) && axis == Y_AXIS)
					{
						Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
						Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = false;
					}
					return true;
				}
			}
		}

		return false;
	}

	bool CollisionSystem::CheckForSectorCollisions(IEntity& p, int index, VertexBuffer& buffer, int axis)
	{
		//FUNCTION_PROFILE();

		for (IEntity& entt : sectors[index])
		{
			bool collision = false;
			Component::CollisionBox& cb_i = Registry::Get().GetComponent<Component::CollisionBox>(entt);

			if (p.uuid != entt.uuid && cb_i.Enabled) 
			{
				Component::CollisionBox& cb_p = Registry::Get().GetComponent<Component::CollisionBox>(p);

				collision = CheckCornerBRTL(buffer, p, cb_p.br, cb_i.tl, cb_i.br, axis);
				if (collision)
				{
					if (Registry::Get().HasComponent<Component::Gravity>(p))
					{
						if (axis == Y_AXIS)
						{
							Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
							Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = true;
						}
					}
					return true;
				}

				collision = CheckCornerTRBL(buffer, p, cb_p.tr, cb_i.bl, cb_i.tr, axis);
				if (collision)
				{
					if (Registry::Get().HasComponent<Component::Gravity>(p))
					{
						if (axis == Y_AXIS)
						{
							Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
							Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = false;
						}
					}
					return true;
				}

				collision = CheckCornerTLBR(buffer, p, cb_p.tl, cb_i.br, cb_i.tl, axis);
				if (collision)
				{
					if (Registry::Get().HasComponent<Component::Gravity>(p))
					{
						if (axis == Y_AXIS)
						{
							Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
							Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = false;
						}
					}
					return true;
				}

				collision = CheckCornerBLTR(buffer, p, cb_p.bl, cb_i.tr, cb_i.bl, axis);
				if (collision)
				{
					if (Registry::Get().HasComponent<Component::Gravity>(p))
					{
						if (axis == Y_AXIS)
						{
							Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
							Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = true;
						}
					}
					return true;
				}

				collision = CheckSideLR(buffer, p, cb_p.br, cb_p.tr, cb_i.bl, cb_i.tl, cb_i.tr, axis);
				if (collision)
				{
					if (Registry::Get().HasComponent<Component::Gravity>(p))
					{
						if (axis == Y_AXIS)
						{
							Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
							Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = true;
						}
					}
					return true;
				}

				collision = CheckSideRL(buffer, p, cb_p.tl, cb_p.bl, cb_i.tr, cb_i.br, cb_i.bl, axis);
				if (collision)
				{
					if (Registry::Get().HasComponent<Component::Gravity>(p))
					{
						if (axis == Y_AXIS)
						{
							Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
							Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = true;
						}
					}
					return true;
				}

				collision = CheckSideTB(buffer, p, cb_p.br, cb_p.bl, cb_i.tr, cb_i.tl, cb_i.bl, axis);
				if (collision)
				{
					if (Registry::Get().HasComponent<Component::Gravity>(p))
					{
						if (axis == Y_AXIS)
						{
							Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
							Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = true;
						}
					}
					return true;
				}

				collision = CheckSideBT(buffer, p, cb_p.tl, cb_p.tr, cb_i.bl, cb_i.br, cb_i.tl, axis);
				if (collision)
				{
					if (Registry::Get().HasComponent<Component::Gravity>(p))
					{
						if (axis == Y_AXIS)
						{
							Registry::Get().GetComponent<Component::Gravity>(p).collides = true;
							Registry::Get().GetComponent<Component::Gravity>(p).isGrounded = false;
						}
					}
					return true;
				}
			}
		}

		return false;
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
