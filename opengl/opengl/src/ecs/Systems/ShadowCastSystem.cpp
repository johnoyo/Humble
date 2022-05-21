#include "ShadowCastSystem.h"

//TODO: improve shadow cast calculation algorithm

void ShadowCastSystem::Start(glm::vec4 shadow_color, glm::vec3 player_position, VertexBuffer& buffer, RenderingSystem& rend)
{
	ENGINE_PROFILE("ShadowCastSystem::Start");

	int offset = 0;
	// Init shadow cast component info
	for (int i = 0; i < entities.size(); i++) {
		if (entities.at(i).Transform != -1 && entities.at(i).Shadow != -1) {
			if (Shadow.at(entities.at(i).Shadow).Enabled) {
				Shadow.at(entities.at(i).Shadow).parentBufferIndex = Transform.at(entities.at(i).Transform).bufferIndex;
				Shadow.at(entities.at(i).Shadow).bufferIndex = buffer.Get_Size() + offset;
				Shadow.at(entities.at(i).Shadow).color = shadow_color;
				offset += 12;
			}
		}
	}

	// Init shadow cast positions
	glm::vec3 O = player_position;

	for (int i = 0; i < Shadow.size(); i++) {
		if (Shadow.at(i).Enabled) {

			std::vector<glm::vec2> shadow_points;
			std::vector<glm::vec2> edge_points;
			std::vector<glm::vec2> vertices;

			// Retrieve vertices of entity
			vertices.push_back(buffer.Get_Buffer()[Shadow.at(i).parentBufferIndex + 0].position);
			vertices.push_back(buffer.Get_Buffer()[Shadow.at(i).parentBufferIndex + 1].position);
			vertices.push_back(buffer.Get_Buffer()[Shadow.at(i).parentBufferIndex + 2].position);
			vertices.push_back(buffer.Get_Buffer()[Shadow.at(i).parentBufferIndex + 3].position);

			// Find all shadow points
			for (int j = 0; j < 4; j++) {

				glm::vec2 E = vertices[j];

				float rdx, rdy;
				rdx = E.x - O.x;
				rdy = E.y - O.y;

				float base_ang = atan2f(rdy, rdx);

				rdx = Shadow.at(i).shadowDistance * cosf(base_ang);
				rdy = Shadow.at(i).shadowDistance * sinf(base_ang);

				shadow_points.push_back({ rdx, rdy });

				// Find Intersection points between shadow line and all edges, store closer one for each edge 
			}

			// Set shadow quad positions
			rend.Draw_Quad(vertices[3], shadow_points[3], shadow_points[0], vertices[0], Shadow.at(i).color);
			rend.Draw_Quad(vertices[0], shadow_points[0], shadow_points[1], vertices[1], Shadow.at(i).color);
			rend.Draw_Quad(vertices[1], shadow_points[1], shadow_points[2], vertices[2], Shadow.at(i).color);
		}
	}

	// Reset rendering buffers
	rend.Invalidate();
}

void ShadowCastSystem::Run(glm::vec3 player_position, VertexBuffer& buffer, RenderingSystem& rend)
{
	glm::vec3 O = player_position;

	for (int i = 0; i < Shadow.size(); i++) {
		if (Shadow.at(i).Enabled) {
			std::vector<glm::vec2> shadow_points;
			std::vector<glm::vec2> edge_points;

			std::vector<glm::vec2> vertices;
			vertices.push_back(buffer.Get_Buffer()[Shadow.at(i).parentBufferIndex + 0].position);
			vertices.push_back(buffer.Get_Buffer()[Shadow.at(i).parentBufferIndex + 1].position);
			vertices.push_back(buffer.Get_Buffer()[Shadow.at(i).parentBufferIndex + 2].position);
			vertices.push_back(buffer.Get_Buffer()[Shadow.at(i).parentBufferIndex + 3].position);

			// Find all shadow points
			for (int j = 0; j < 4; j++) {

				glm::vec2 E = vertices[j];

				float rdx, rdy;
				rdx = E.x - O.x;
				rdy = E.y - O.y;

				float base_ang = atan2f(rdy, rdx);

				rdx = Shadow.at(i).shadowDistance * cosf(base_ang);
				rdy = Shadow.at(i).shadowDistance * sinf(base_ang);

				shadow_points.push_back({ rdx, rdy });

				// Find Intersection points between shadow line and all edges, store closer one for each edge 
			}

			// Update shadow quad positions
			buffer.Update_Position_On_Quad(Shadow.at(i).bufferIndex, vertices[3], shadow_points[3], shadow_points[0], vertices[0]);
			buffer.Update_Position_On_Quad(Shadow.at(i).bufferIndex + 4, vertices[0], shadow_points[0], shadow_points[1], vertices[1]);
			buffer.Update_Position_On_Quad(Shadow.at(i).bufferIndex + 8, vertices[1], shadow_points[1], shadow_points[2], vertices[2]);
		}
	}
}
