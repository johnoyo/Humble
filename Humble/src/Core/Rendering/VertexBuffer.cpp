#include "VertexBuffer.h"

namespace HBL 
{
	void VertexBuffer::Initialize(int total_m_Buffer_size)
	{
		total_size = total_m_Buffer_size;
		m_Buffer = (struct Vertex_Array*)malloc(total_m_Buffer_size * sizeof(struct Vertex_Array));
	}

	void VertexBuffer::FillBuffer(glm::vec2 position, glm::vec4 color, glm::vec2 tex_coords, float tex_id)
	{
		m_Buffer[m_Index].position = position;
		m_Buffer[m_Index].color = color;
		m_Buffer[m_Index].tex_coord = tex_coords;
		m_Buffer[m_Index].tex_id = tex_id;
		m_Index++;
	}

	void VertexBuffer::UpdatePositionOnQuad(uint32_t indx, Component::Transform& tr)
	{
		//FUNCTION_PROFILE();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), tr.position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(tr.rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(tr.scale.x, tr.scale.y, 1.0f));

		glm::vec4 quad_vertex_position[4];

		quad_vertex_position[0] = { -0.5f,  0.5f, 0.0f, 1.0f };
		quad_vertex_position[1] = {  0.5f,  0.5f, 0.0f, 1.0f };
		quad_vertex_position[2] = {  0.5f, -0.5f, 0.0f, 1.0f };
		quad_vertex_position[3] = { -0.5f, -0.5f, 0.0f, 1.0f };

		m_Buffer[indx].position = transform * quad_vertex_position[0];
		indx++;
		m_Buffer[indx].position = transform * quad_vertex_position[1];
		indx++;
		m_Buffer[indx].position = transform * quad_vertex_position[2];
		indx++;
		m_Buffer[indx].position = transform * quad_vertex_position[3];
		indx++;
	}

	void VertexBuffer::UpdatePositionOnQuad(uint32_t indx, glm::vec3& position, float rotation, glm::vec3& scale)
	{
		//FUNCTION_PROFILE();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

		glm::vec4 quad_vertex_position[4];

		quad_vertex_position[0] = { -0.5f,  0.5f, 0.0f, 1.0f };
		quad_vertex_position[1] = {  0.5f,  0.5f, 0.0f, 1.0f };
		quad_vertex_position[2] = {  0.5f, -0.5f, 0.0f, 1.0f };
		quad_vertex_position[3] = { -0.5f, -0.5f, 0.0f, 1.0f };

		m_Buffer[indx].position = transform * quad_vertex_position[0];
		indx++;
		m_Buffer[indx].position = transform * quad_vertex_position[1];
		indx++;
		m_Buffer[indx].position = transform * quad_vertex_position[2];
		indx++;
		m_Buffer[indx].position = transform * quad_vertex_position[3];
		indx++;
	}

	void VertexBuffer::UpdatePositionOnQuad(uint32_t indx, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
	{
		m_Buffer[indx].position = p0;
		indx++;
		m_Buffer[indx].position = p1;
		indx++;
		m_Buffer[indx].position = p2;
		indx++;
		m_Buffer[indx].position = p3;
		indx++;
	}

	void VertexBuffer::UpdatePositionXOnQuad(uint32_t indx, Component::Transform& tr)
	{
		m_Buffer[indx].position.x = tr.position.x - tr.scale.x / 2.0f;
		indx++;
		m_Buffer[indx].position.x = tr.position.x + tr.scale.x / 2.0f;
		indx++;
		m_Buffer[indx].position.x = tr.position.x + tr.scale.x / 2.0f;
		indx++;
		m_Buffer[indx].position.x = tr.position.x - tr.scale.x / 2.0f;
		indx++;
	}

	void VertexBuffer::UpdatePositionYOnQuad(uint32_t indx, Component::Transform& tr)
	{
		m_Buffer[indx].position.y = tr.position.y + tr.scale.y / 2.0f;
		indx++;
		m_Buffer[indx].position.y = tr.position.y + tr.scale.y / 2.0f;
		indx++;
		m_Buffer[indx].position.y = tr.position.y - tr.scale.y / 2.0f;
		indx++;
		m_Buffer[indx].position.y = tr.position.y - tr.scale.y / 2.0f;
		indx++;
	}

	void VertexBuffer::UpdateMaterialOnQuad(uint32_t indx, glm::vec4& color, float tex_id)
	{
		m_Buffer[indx].color = color;
		m_Buffer[indx].tex_coord = { 0.0f, 1.0f };
		m_Buffer[indx].tex_id = tex_id;
		indx++;
		m_Buffer[indx].color = color;
		m_Buffer[indx].tex_coord = { 1.0f, 1.0f };
		m_Buffer[indx].tex_id = tex_id;
		indx++;
		m_Buffer[indx].color = color;
		m_Buffer[indx].tex_coord = { 1.0f, 0.0f };
		m_Buffer[indx].tex_id = tex_id;
		indx++;
		m_Buffer[indx].color = color;
		m_Buffer[indx].tex_coord = { 0.0f, 0.0f };
		m_Buffer[indx].tex_id = tex_id;
		indx++;
	}

	void VertexBuffer::UpdateMaterialOnQuad(uint32_t indx, glm::vec4& color, float tex_id, glm::vec2& coords, glm::vec2& sheet_size, glm::vec2& sp_size)
	{
		m_Buffer[indx].color = color;
		m_Buffer[indx].tex_coord = { (coords.x * sp_size.x) / sheet_size.x, ((coords.y + 1) * sp_size.y) / sheet_size.y };
		m_Buffer[indx].tex_id = tex_id;
		indx++;
		m_Buffer[indx].color = color;
		m_Buffer[indx].tex_coord = { ((coords.x + 1) * sp_size.x) / sheet_size.x, ((coords.y + 1) * sp_size.y) / sheet_size.y };
		m_Buffer[indx].tex_id = tex_id;
		indx++;
		m_Buffer[indx].color = color;
		m_Buffer[indx].tex_coord = { ((coords.x + 1) * sp_size.x) / sheet_size.x, (coords.y * sp_size.y) / sheet_size.y };
		m_Buffer[indx].tex_id = tex_id;
		indx++;
		m_Buffer[indx].color = color;
		m_Buffer[indx].tex_coord = { (coords.x * sp_size.x) / sheet_size.x, (coords.y * sp_size.y) / sheet_size.y };
		m_Buffer[indx].tex_id = tex_id;
		indx++;
	}

	Vertex_Array* VertexBuffer::GetBuffer()
	{
		return m_Buffer;
	}

	void VertexBuffer::SetBuffer(Vertex_Array* new_m_Buffer)
	{
		m_Buffer = new_m_Buffer;
	}

	uint32_t VertexBuffer::GetSize()
	{
		return m_Index;
	}

	void VertexBuffer::SetSize(uint32_t size)
	{
		m_Index = size;
	}

	uint32_t VertexBuffer::GetTotalSize()
	{
		return total_size;
	}

	void VertexBuffer::Reset()
	{
		m_Index = 0;
	}

	void VertexBuffer::Clean()
	{
		if (m_Buffer != NULL) free(m_Buffer);
		total_size = 0;
		m_Index = 0;
	}
}