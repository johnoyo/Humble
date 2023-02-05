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
		Registry::Get().Group<Component::Transform, Component::SpriteRenderer, Component::Shadow>().ForEach([&](IEntity& entt)
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

				// Find all shadow points
				for (int j = 0; j < 4; j++)
				{
					glm::vec2& E = buffer.GetBuffer()[shadow.parentBufferIndex + j].position;

					float rdx, rdy;
					rdx = E.x - O.x;
					rdy = E.y - O.y;

					float base_ang = atan2f(rdy, rdx);

					rdx = shadow.shadowDistance * cosf(base_ang);
					rdy = shadow.shadowDistance * sinf(base_ang);

					shadow.points.push_back({ rdx, rdy });
				}

				// Set shadow quad positions
				Renderer::Get().RegisterQuad(0,
												buffer.GetBuffer()[shadow.parentBufferIndex + 3].position,
												shadow.points[3], shadow.points[0],
												buffer.GetBuffer()[shadow.parentBufferIndex + 0].position,
												shadow.color);

				Renderer::Get().RegisterQuad(0,
												buffer.GetBuffer()[shadow.parentBufferIndex + 0].position,
												shadow.points[0], shadow.points[1],
												buffer.GetBuffer()[shadow.parentBufferIndex + 1].position,
												shadow.color);

				Renderer::Get().RegisterQuad(0,
												buffer.GetBuffer()[shadow.parentBufferIndex + 1].position,
												shadow.points[1],
												shadow.points[2],
												buffer.GetBuffer()[shadow.parentBufferIndex + 2].position, shadow.color);
			}
		}).Run();
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

				// Find all shadow points
				for (int j = 0; j < 4; j++)
				{
					glm::vec2& E = buffer.GetBuffer()[shadow.parentBufferIndex + j].position;

					float rdx, rdy;
					rdx = E.x - O.x;
					rdy = E.y - O.y;

					float base_ang = atan2f(rdy, rdx);

					rdx = (shadow.shadowDistance) * cosf(base_ang);
					rdy = (shadow.shadowDistance) * sinf(base_ang);

					shadow.points[j] = { rdx, rdy };
				}

				// Update shadow quad positions and colors.
				buffer.UpdatePositionOnQuad(shadow.bufferIndex,
											buffer.GetBuffer()[shadow.parentBufferIndex + 3].position,
											shadow.points[3],
											shadow.points[0],
											buffer.GetBuffer()[shadow.parentBufferIndex + 0].position);

				buffer.UpdateMaterialOnQuad(shadow.bufferIndex, shadow.color, 0);

				buffer.UpdatePositionOnQuad(shadow.bufferIndex + 4,
											buffer.GetBuffer()[shadow.parentBufferIndex + 0].position,
											shadow.points[0],
											shadow.points[1],
											buffer.GetBuffer()[shadow.parentBufferIndex + 1].position);

				buffer.UpdateMaterialOnQuad(shadow.bufferIndex + 4, shadow.color, 0);

				buffer.UpdatePositionOnQuad(shadow.bufferIndex + 8,
											buffer.GetBuffer()[shadow.parentBufferIndex + 1].position,
											shadow.points[1],
											shadow.points[2],
											buffer.GetBuffer()[shadow.parentBufferIndex + 2].position);

				buffer.UpdateMaterialOnQuad(shadow.bufferIndex + 8, shadow.color, 0);
			}
			else
			{
				if (shadow.bufferIndex != -1)
				{
					glm::vec4 transparentColor(1.f, 1.f, 1.f, 0.f);

					buffer.UpdateMaterialOnQuad(shadow.bufferIndex, transparentColor, 0);
					buffer.UpdateMaterialOnQuad(shadow.bufferIndex + 4, transparentColor, 0);
					buffer.UpdateMaterialOnQuad(shadow.bufferIndex + 8, transparentColor, 0);
				}
			}
		}).Run();
	}

	void ShadowCastSystem::Clear()
	{
		Registry::Get().ClearArray<Component::Shadow>();
	}

}
