#include "ShadowCastSystem.h"
#include "../Systems.h"

namespace HBL 
{
	void ShadowCastSystem::Start()
	{
		FUNCTION_PROFILE();

		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		uint32_t offset = 0;

		// Init shadow cast component info
		Registry::Get().Filter<Component::Transform, Component::SpriteRenderer, Component::Shadow>().ForEach([&](IEntity& entt)
		{
			Component::Shadow& shadow = Registry::Get().GetComponent<Component::Shadow>(entt);

			if (shadow.Enabled && shadow.source != nullptr)
			{
				Component::SpriteRenderer& sprite = Registry::Get().GetComponent<Component::SpriteRenderer>(entt);

				sprite.texture = "-";
				sprite.color = shadow.color;

				shadow.parentBufferIndex = Registry::Get().GetComponent<Component::Transform>(entt).bufferIndex;
				shadow.bufferIndex = buffer.GetSize() + offset;

				offset += 12;
			}
		}).Run();

		// Init shadow cast positions
		Registry::Get().View<Component::Shadow>().ForEach([&](Component::Shadow& shadow)
		{
			if (shadow.Enabled && shadow.source != nullptr)
			{
				glm::vec3& O = Registry::Get().GetComponent<Component::Transform>(*shadow.source).position;

				std::vector<glm::vec2> vertices;

				// Retrieve vertices of entity
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 0].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 1].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 2].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 3].position);

				std::vector<glm::vec2> shadow_points;

				// Find all shadow points
				for (int j = 0; j < 4; j++)
				{
					glm::vec2& E = vertices[j];

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
		}).Run();

		// Reset rendering buffers
		Renderer::Get().Bind(0);
		Renderer::Get().Invalidate(0);
		Renderer::Get().UnBind();
	}

	void ShadowCastSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		Registry::Get().View<Component::Shadow>().ForEach([&](Component::Shadow& shadow)
		{
			if (shadow.Enabled && shadow.source != nullptr)
			{
				glm::vec3& O = Registry::Get().GetComponent<Component::Transform>(*shadow.source).position;

				std::vector<glm::vec2> vertices;
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 0].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 1].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 2].position);
				vertices.push_back(buffer.GetBuffer()[shadow.parentBufferIndex + 3].position);

				std::vector<glm::vec2> shadow_points;

				// Find all shadow points
				for (int j = 0; j < 4; j++)
				{
					glm::vec2& E = vertices[j];

					float rdx, rdy;
					rdx = E.x - O.x;
					rdy = E.y - O.y;

					float base_ang = atan2f(rdy, rdx);

					rdx = (shadow.shadowDistance) * cosf(base_ang);
					rdy = (shadow.shadowDistance) * sinf(base_ang);

					shadow_points.push_back({ rdx, rdy });
				}

				// Update shadow quad positions and colors.
				buffer.UpdatePositionOnQuad(shadow.bufferIndex, vertices[3], shadow_points[3], shadow_points[0], vertices[0]);
				buffer.UpdateMaterialOnQuad(shadow.bufferIndex, shadow.color, 0);

				buffer.UpdatePositionOnQuad(shadow.bufferIndex + 4, vertices[0], shadow_points[0], shadow_points[1], vertices[1]);
				buffer.UpdateMaterialOnQuad(shadow.bufferIndex + 4, shadow.color, 0);

				buffer.UpdatePositionOnQuad(shadow.bufferIndex + 8, vertices[1], shadow_points[1], shadow_points[2], vertices[2]);
				buffer.UpdateMaterialOnQuad(shadow.bufferIndex + 8, shadow.color, 0);
			}
		}).Run();
	}

	void ShadowCastSystem::Clear()
	{
		Registry::Get().ClearArray<Component::Shadow>();
	}

}
