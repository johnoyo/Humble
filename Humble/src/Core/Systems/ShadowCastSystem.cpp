#include "ShadowCastSystem.h"
#include "../GlobalSystems.h"

namespace HBL {

	void ShadowCastSystem::Start(glm::vec4& shadow_color, glm::vec3& player_position)
	{
		FUNCTION_PROFILE();

		SpriteRendererSystem& rend = GlobalSystems::spriteRendererSystem;
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		uint32_t offset = 0;
		// Init shadow cast component info
		for (uint32_t i = 0; i < Globals::s_Registry.GetEntities().size(); i++) 
		{
			IEntity& entt = Globals::s_Registry.GetEntities().at(i);

			if (Globals::s_Registry.HasComponent<Component::Transform>(entt) &&
				Globals::s_Registry.HasComponent<Component::SpriteRenderer>(entt) &&
				Globals::s_Registry.HasComponent<Component::Shadow>(entt)) 
			{
				Component::Shadow& shadow = Globals::s_Registry.GetComponent<Component::Shadow>(entt);

				if (shadow.Enabled)
				{
					Component::SpriteRenderer& sprite = Globals::s_Registry.GetComponent<Component::SpriteRenderer>(entt);

					sprite.texture = "-";
					sprite.color = shadow_color;

					shadow.parentBufferIndex = Globals::s_Registry.GetComponent<Component::Transform>(entt).bufferIndex;
					shadow.bufferIndex = buffer.GetSize() + offset;
					shadow.color = shadow_color;

					offset += 12;
				}
			}
		}

		// Init shadow cast positions
		glm::vec3 O = player_position;

		for (auto& component : Globals::s_Registry.GetArray<Component::Shadow>())
		{
			Component::Shadow& shadow = component.second;

			if (shadow.Enabled)
			{
				std::vector<glm::vec2> shadow_points;
				std::vector<glm::vec2> edge_points;
				std::vector<glm::vec2> vertices;

				// Retrieve vertices of entity
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 0].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 1].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 2].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 3].position);

				// Find all shadow points
				for (int j = 0; j < 4; j++) 
				{
					glm::vec2 E = vertices[j];

					float rdx, rdy;
					rdx = E.x - O.x;
					rdy = E.y - O.y;

					float base_ang = atan2f(rdy, rdx);

					rdx = shadow.shadowDistance * cosf(base_ang);
					rdy = shadow.shadowDistance * sinf(base_ang);

					shadow_points.push_back({ rdx, rdy });
				}

				// Set shadow quad positions
				Renderer::Get().DrawQuad(0, vertices[3], shadow_points[3], shadow_points[0], vertices[0], shadow.color);
				Renderer::Get().DrawQuad(0, vertices[0], shadow_points[0], shadow_points[1], vertices[1], shadow.color);
				Renderer::Get().DrawQuad(0, vertices[1], shadow_points[1], shadow_points[2], vertices[2], shadow.color);
			}
		}

		// Reset rendering buffers
		Renderer::Get().Bind(0);
		Renderer::Get().Invalidate(0);
		Renderer::Get().UnBind();
	}

	void ShadowCastSystem::Run(glm::vec3& player_position)
	{
		//FUNCTION_PROFILE();

		SpriteRendererSystem& rend = GlobalSystems::spriteRendererSystem;
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		glm::vec3 O = player_position;

		for (auto& component : Globals::s_Registry.GetArray<Component::Shadow>())
		{
			Component::Shadow& shadow = component.second;

			if (shadow.Enabled)
			{
				std::vector<glm::vec2> shadow_points;
				std::vector<glm::vec2> edge_points;

				std::vector<glm::vec2> vertices;
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 0].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 1].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 2].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 3].position);

				// Find all shadow points
				for (int j = 0; j < 4; j++) 
				{
					glm::vec2 E = vertices[j];

					float rdx, rdy;
					rdx = E.x - O.x;
					rdy = E.y - O.y;

					float base_ang = atan2f(rdy, rdx);

					rdx = (shadow.shadowDistance) * cosf(base_ang);
					rdy = (shadow.shadowDistance) * sinf(base_ang);

					shadow_points.push_back({ rdx, rdy });

					// Find Intersection points between shadow line and all edges, store closer one for each edge 
				}

				// Update shadow quad positions
				buffer.UpdatePositionOnQuad(shadow.bufferIndex, vertices[3], shadow_points[3], shadow_points[0], vertices[0]);
				buffer.UpdatePositionOnQuad(shadow.bufferIndex + 4, vertices[0], shadow_points[0], shadow_points[1], vertices[1]);
				buffer.UpdatePositionOnQuad(shadow.bufferIndex + 8, vertices[1], shadow_points[1], shadow_points[2], vertices[2]);
			}
		}
	}

	void ShadowCastSystem::Clear()
	{
		Globals::s_Registry.GetArray<Component::Shadow>().clear();
	}

}
