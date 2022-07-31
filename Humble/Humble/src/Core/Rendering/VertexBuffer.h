#pragma once
#include "../Core.h"
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"

namespace HBL {

	struct Vertex_Array {
		glm::vec2 position;
		glm::vec4 color;
		glm::vec2 tex_coord;
		float tex_id;
	};

	class HBL_API VertexBuffer {
	public:
		void Initialize(int total_buffer_size);

		void Update_Position_On_Quad(uint32_t indx, Component::Transform& tr);
		void Update_Position_On_Quad(uint32_t indx, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
		void Update_PositionX_On_Quad(uint32_t indx, Component::Transform& tr);
		void Update_PositionY_On_Quad(uint32_t indx, Component::Transform& tr);
		void Update_Material_On_Quad(uint32_t indx, glm::vec4& color, float tex_id);
		void Update_Material_On_Quad(uint32_t indx, glm::vec4& color, float tex_id, glm::vec2& coords, glm::vec2& sheet_size, glm::vec2& sp_size);

		void Set_Buffer(struct Vertex_Array* new_buffer);
		struct Vertex_Array* Get_Buffer();
		uint32_t Get_Size();
		void Set_Size(uint32_t size);
		uint32_t Get_Total_Size();
		void Reset();
		void Clean();

		uint32_t total_size = 0;
		friend class Renderer;
		friend class RenderingSystem;

	private:
		struct Vertex_Array* buffer = NULL;
		uint32_t index = 0;
		void Fill_Buffer(glm::vec2 position, glm::vec4 color, glm::vec2 tex_coords, float tex_id);
	};

}