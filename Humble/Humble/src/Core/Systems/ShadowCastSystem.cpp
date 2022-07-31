#include "ShadowCastSystem.h"
#include "../SystemsHeader.h"

namespace HBL {

	void ShadowCastSystem::Start(glm::vec4& shadow_color, glm::vec3& player_position)
	{
		FUNCTION_PROFILE();

		RenderingSystem& rend = GlobalSystems::renderingSystem;
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		uint32_t offset = 0;
		// Init shadow cast component info
		for (uint32_t i = 0; i < Globals::entities.size(); i++) {
			if (TRY_FIND_COMPONENT(Transform, Globals::entities.at(i)) && TRY_FIND_COMPONENT(Material, Globals::entities.at(i)) && TRY_FIND_COMPONENT(Shadow, Globals::entities.at(i))) {
				if (GET_COMPONENT(Shadow, Globals::entities.at(i)).Enabled) {
					GET_COMPONENT(Material, Globals::entities.at(i)).texture = "-";
					GET_COMPONENT(Material, Globals::entities.at(i)).color = shadow_color;

					GET_COMPONENT(Shadow, Globals::entities.at(i)).parentBufferIndex = GET_COMPONENT(Transform, Globals::entities.at(i)).bufferIndex;
					GET_COMPONENT(Shadow, Globals::entities.at(i)).bufferIndex = buffer.Get_Size() + offset;
					GET_COMPONENT(Shadow, Globals::entities.at(i)).color = shadow_color;
					offset += 12;
				}
			}
		}

		// Init shadow cast positions
		glm::vec3 O = player_position;

		for (uint32_t i = 0; i < Globals::Shadow.size(); i++) {
			if (Globals::Shadow.at(i).Enabled) {

				std::vector<glm::vec2> shadow_points;
				std::vector<glm::vec2> edge_points;
				std::vector<glm::vec2> vertices;

				// Retrieve vertices of entity
				vertices.push_back(buffer.Get_Buffer()[Globals::Shadow.at(i).parentBufferIndex + 0].position);
				vertices.push_back(buffer.Get_Buffer()[Globals::Shadow.at(i).parentBufferIndex + 1].position);
				vertices.push_back(buffer.Get_Buffer()[Globals::Shadow.at(i).parentBufferIndex + 2].position);
				vertices.push_back(buffer.Get_Buffer()[Globals::Shadow.at(i).parentBufferIndex + 3].position);

				// Find all shadow points
				for (int j = 0; j < 4; j++) {

					glm::vec2 E = vertices[j];

					float rdx, rdy;
					rdx = E.x - O.x;
					rdy = E.y - O.y;

					float base_ang = atan2f(rdy, rdx);

					rdx = Globals::Shadow.at(i).shadowDistance * cosf(base_ang);
					rdy = Globals::Shadow.at(i).shadowDistance * sinf(base_ang);

					shadow_points.push_back({ rdx, rdy });

				}

				// Set shadow quad positions
				Renderer::Get().Draw_Quad(0, vertices[3], shadow_points[3], shadow_points[0], vertices[0], Globals::Shadow.at(i).color);
				Renderer::Get().Draw_Quad(0, vertices[0], shadow_points[0], shadow_points[1], vertices[1], Globals::Shadow.at(i).color);
				Renderer::Get().Draw_Quad(0, vertices[1], shadow_points[1], shadow_points[2], vertices[2], Globals::Shadow.at(i).color);
			}
		}

		// Reset rendering buffers
		Renderer::Get().Invalidate(0);
	}

	void ShadowCastSystem::Run(glm::vec3& player_position)
	{
		//FUNCTION_PROFILE();

		RenderingSystem& rend = GlobalSystems::renderingSystem;
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		glm::vec3 O = player_position;

		for (uint32_t i = 0; i < Globals::Shadow.size(); i++) {
			if (Globals::Shadow.at(i).Enabled) {
				std::vector<glm::vec2> shadow_points;
				std::vector<glm::vec2> edge_points;

				std::vector<glm::vec2> vertices;
				vertices.push_back(buffer.Get_Buffer()[Globals::Shadow.at(i).parentBufferIndex + 0].position);
				vertices.push_back(buffer.Get_Buffer()[Globals::Shadow.at(i).parentBufferIndex + 1].position);
				vertices.push_back(buffer.Get_Buffer()[Globals::Shadow.at(i).parentBufferIndex + 2].position);
				vertices.push_back(buffer.Get_Buffer()[Globals::Shadow.at(i).parentBufferIndex + 3].position);

				// Find all shadow points
				for (int j = 0; j < 4; j++) {

					glm::vec2 E = vertices[j];

					float rdx, rdy;
					rdx = E.x - O.x;
					rdy = E.y - O.y;

					float base_ang = atan2f(rdy, rdx);

					rdx = (Globals::Shadow.at(i).shadowDistance) * cosf(base_ang);
					rdy = (Globals::Shadow.at(i).shadowDistance) * sinf(base_ang);

					shadow_points.push_back({ rdx, rdy });

					// Find Intersection points between shadow line and all edges, store closer one for each edge 
				}

				// Update shadow quad positions
				buffer.Update_Position_On_Quad(Globals::Shadow.at(i).bufferIndex, vertices[3], shadow_points[3], shadow_points[0], vertices[0]);
				buffer.Update_Position_On_Quad(Globals::Shadow.at(i).bufferIndex + 4, vertices[0], shadow_points[0], shadow_points[1], vertices[1]);
				buffer.Update_Position_On_Quad(Globals::Shadow.at(i).bufferIndex + 8, vertices[1], shadow_points[1], shadow_points[2], vertices[2]);
			}
		}
	}

	void ShadowCastSystem::Clear()
	{
		Globals::Shadow.clear();
	}

}
