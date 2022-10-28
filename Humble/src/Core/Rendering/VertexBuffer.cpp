#include "VertexBuffer.h"

namespace HBL {

	void VertexBuffer::Initialize(int total_buffer_size)
	{
		total_size = total_buffer_size;
		buffer = (struct Vertex_Array*)malloc(total_buffer_size * sizeof(struct Vertex_Array));
	}

	void VertexBuffer::Fill_Buffer(glm::vec2 position, glm::vec4 color, glm::vec2 tex_coords, float tex_id)
	{
		buffer[index].position = position;
		buffer[index].color = color;
		buffer[index].tex_coord = tex_coords;
		buffer[index].tex_id = tex_id;
		index++;
	}

	void VertexBuffer::Update_Position_On_Quad(uint32_t indx, Component::Transform& tr)
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

		buffer[indx].position = transform * quad_vertex_position[0];
		indx++;
		buffer[indx].position = transform * quad_vertex_position[1];
		indx++;
		buffer[indx].position = transform * quad_vertex_position[2];
		indx++;
		buffer[indx].position = transform * quad_vertex_position[3];
		indx++;
	}

	void VertexBuffer::Update_Position_On_Quad(uint32_t indx, glm::vec3& position, float rotation, glm::vec3& scale)
	{
		//FUNCTION_PROFILE();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

		glm::vec4 quad_vertex_position[4];

		quad_vertex_position[0] = { -0.5f,  0.5f, 0.0f, 1.0f };
		quad_vertex_position[1] = { 0.5f,  0.5f, 0.0f, 1.0f };
		quad_vertex_position[2] = { 0.5f, -0.5f, 0.0f, 1.0f };
		quad_vertex_position[3] = { -0.5f, -0.5f, 0.0f, 1.0f };

		buffer[indx].position = transform * quad_vertex_position[0];
		indx++;
		buffer[indx].position = transform * quad_vertex_position[1];
		indx++;
		buffer[indx].position = transform * quad_vertex_position[2];
		indx++;
		buffer[indx].position = transform * quad_vertex_position[3];
		indx++;
	}

	void VertexBuffer::Update_Position_On_Quad(uint32_t indx, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
	{
		buffer[indx].position = p0;
		indx++;
		buffer[indx].position = p1;
		indx++;
		buffer[indx].position = p2;
		indx++;
		buffer[indx].position = p3;
		indx++;
	}

	void VertexBuffer::Update_PositionX_On_Quad(uint32_t indx, Component::Transform& tr)
	{
		buffer[indx].position.x = tr.position.x - tr.scale.x / 2.0f;
		indx++;
		buffer[indx].position.x = tr.position.x + tr.scale.x / 2.0f;
		indx++;
		buffer[indx].position.x = tr.position.x + tr.scale.x / 2.0f;
		indx++;
		buffer[indx].position.x = tr.position.x - tr.scale.x / 2.0f;
		indx++;
	}

	void VertexBuffer::Update_PositionY_On_Quad(uint32_t indx, Component::Transform& tr)
	{
		buffer[indx].position.y = tr.position.y + tr.scale.y / 2.0f;
		indx++;
		buffer[indx].position.y = tr.position.y + tr.scale.y / 2.0f;
		indx++;
		buffer[indx].position.y = tr.position.y - tr.scale.y / 2.0f;
		indx++;
		buffer[indx].position.y = tr.position.y - tr.scale.y / 2.0f;
		indx++;
	}

	void VertexBuffer::Update_Material_On_Quad(uint32_t indx, glm::vec4& color, float tex_id)
	{
		buffer[indx].color = color;
		buffer[indx].tex_coord = { 0.0f, 1.0f };
		buffer[indx].tex_id = tex_id;
		indx++;
		buffer[indx].color = color;
		buffer[indx].tex_coord = { 1.0f, 1.0f };
		buffer[indx].tex_id = tex_id;
		indx++;
		buffer[indx].color = color;
		buffer[indx].tex_coord = { 1.0f, 0.0f };
		buffer[indx].tex_id = tex_id;
		indx++;
		buffer[indx].color = color;
		buffer[indx].tex_coord = { 0.0f, 0.0f };
		buffer[indx].tex_id = tex_id;
		indx++;
	}

	void VertexBuffer::Update_Material_On_Quad(uint32_t indx, glm::vec4& color, float tex_id, glm::vec2& coords, glm::vec2& sheet_size, glm::vec2& sp_size)
	{
		buffer[indx].color = color;
		buffer[indx].tex_coord = { (coords.x * sp_size.x) / sheet_size.x, ((coords.y + 1) * sp_size.y) / sheet_size.y };
		buffer[indx].tex_id = tex_id;
		indx++;
		buffer[indx].color = color;
		buffer[indx].tex_coord = { ((coords.x + 1) * sp_size.x) / sheet_size.x, ((coords.y + 1) * sp_size.y) / sheet_size.y };
		buffer[indx].tex_id = tex_id;
		indx++;
		buffer[indx].color = color;
		buffer[indx].tex_coord = { ((coords.x + 1) * sp_size.x) / sheet_size.x, (coords.y * sp_size.y) / sheet_size.y };
		buffer[indx].tex_id = tex_id;
		indx++;
		buffer[indx].color = color;
		buffer[indx].tex_coord = { (coords.x * sp_size.x) / sheet_size.x, (coords.y * sp_size.y) / sheet_size.y };
		buffer[indx].tex_id = tex_id;
		indx++;
	}

	Vertex_Array* VertexBuffer::Get_Buffer()
	{
		return buffer;
	}

	void VertexBuffer::Set_Buffer(Vertex_Array* new_buffer)
	{
		buffer = new_buffer;
	}

	uint32_t VertexBuffer::Get_Size()
	{
		return index;
	}

	void VertexBuffer::Set_Size(uint32_t size)
	{
		index = size;
	}

	uint32_t VertexBuffer::Get_Total_Size()
	{
		return total_size;
	}

	void VertexBuffer::Reset()
	{
		index = 0;
	}

	void VertexBuffer::Clean()
	{
		if (buffer != NULL) free(buffer);
		total_size = 0;
		index = 0;
	}

}