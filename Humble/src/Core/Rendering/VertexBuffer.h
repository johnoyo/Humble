#pragma once
#include "../Core.h"
#include "../GlobalArrays.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"

namespace HBL {

	struct Vertex_Array 
	{
		glm::vec2 position;
		glm::vec4 color;
		glm::vec2 tex_coord;
		float tex_id;
	};

	class HBL_API VertexBuffer 
	{
	public:
		void Initialize(int total_buffer_size);

		void UpdatePositionOnQuad(uint32_t indx, Component::Transform& tr);
		void UpdatePositionOnQuad(uint32_t indx, glm::vec3& position, float rotation, glm::vec3& scale);
		void UpdatePositionOnQuad(uint32_t indx, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
		void UpdatePositionXOnQuad(uint32_t indx, Component::Transform& tr);
		void UpdatePositionYOnQuad(uint32_t indx, Component::Transform& tr);
		void UpdateMaterialOnQuad(uint32_t indx, glm::vec4& color, float tex_id);
		void UpdateMaterialOnQuad(uint32_t indx, glm::vec4& color, float tex_id, glm::vec2& coords, glm::vec2& sheet_size, glm::vec2& sp_size);

		void SetBuffer(struct Vertex_Array* new_buffer);
		struct Vertex_Array* GetBuffer();
		uint32_t GetSize();
		void SetSize(uint32_t size);
		uint32_t GetTotalSize();
		void Reset();
		void Clean();

		uint32_t total_size = 0;
		friend class Renderer;
		friend class SpriteRendererSystem;

	private:
		struct Vertex_Array* m_Buffer = nullptr;
		uint32_t m_Index = 0;
		void FillBuffer(glm::vec2 position, glm::vec4 color, glm::vec2 tex_coords, float tex_id);
	};

}