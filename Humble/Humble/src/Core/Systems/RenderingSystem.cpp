#include "RenderingSystem.h"

namespace HBL {

	void RenderingSystem::Prepare(const glm::mat4& m_Camera_vp)
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* vertex array object */
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		/* vertex buffer */
		GLCall(glGenBuffers(1, &vb));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
		GLCall(glBufferData(GL_ARRAY_BUFFER, vbuffer.Get_Total_Size() * sizeof(struct Vertex_Array), nullptr, GL_DYNAMIC_DRAW));

		/* vertex attrib positions*/
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, position)));

		/* vertex attrib colors*/
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, color)));

		/* vertex attrib texture coordinates*/
		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, tex_coord)));

		/* vertex attrib texture id*/
		GLCall(glEnableVertexAttribArray(3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, tex_id)));

		/* index buffer */
		GLCall(glGenBuffers(1, &ib));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (vbuffer.Get_Size() / 4) * 6 * sizeof(uint32_t), ibuffer.Get_Index_Buffer(), GL_STATIC_DRAW));

		/* shaders */
		ShaderProgramSource shaderSource = ParseShader("res/shaders/Basic.shader");
		shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
		GLCall(glUseProgram(shader));

		GLCall(auto loc = glGetUniformLocation(shader, "u_textures"));
		int samplers[32];
		for (uint32_t i = 0; i < 32; i++)
			samplers[i] = i;
		GLCall(glUniform1iv(loc, 32, samplers));

		GLCall(int location1 = glGetUniformLocation(shader, "u_VP"));
		if (location1 == -1) {
			ENGINE_LOG("Uniform not found!!!");
		}
		GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(m_Camera_vp)));
	}

	void RenderingSystem::Render(const glm::mat4& m_Camera_vp)
	{
		Update_Camera_Uniform(m_Camera_vp);

		/* set dynamic vertex buffer */
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vbuffer.Get_Size() * sizeof(Vertex_Array), vbuffer.Get_Buffer()));

		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glDrawElements(GL_TRIANGLES, (vbuffer.Get_Size() / 4) * 6, GL_UNSIGNED_INT, NULL));
	}

	void RenderingSystem::Clear()
	{
		GLCall(glDeleteBuffers(1, &vb));
		GLCall(glDeleteBuffers(1, &ib));
		GLCall(glDeleteProgram(shader));
		GLCall(glDeleteVertexArrays(1, &vao));

		vbuffer.Clean();
		ibuffer.Clean();
	}

	void RenderingSystem::Clear_Buffers()
	{
		vbuffer.Clean();
		ibuffer.Clean();
	}

	void RenderingSystem::Update_Index_Buffer(uint32_t size)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size / 4) * 6 * sizeof(uint32_t), ibuffer.Get_Index_Buffer(), GL_STATIC_DRAW));
	}

	void RenderingSystem::Init_Vertex_Buffer()
	{
		vbuffer.Reset();

		ENGINE_LOG("Transform size: %d", Globals::Transform.size());

		for (uint32_t i = 0; i < Globals::Transform.size(); i++) {
			if (Globals::Transform.at(i).Enabled) {
				Globals::Transform.at(i).bufferIndex = vbuffer.index;
				Draw_Quad(i);
			}
		}

		ENGINE_LOG("Vertex buffer size: %d", vbuffer.Get_Size() / 4);

		ibuffer.Clean();
		ibuffer.Make_Indecies(vbuffer.Get_Size());
		Update_Index_Buffer(vbuffer.Get_Size());
	}

	void RenderingSystem::Update_Camera_Uniform(const glm::mat4& m_Camera_vp)
	{
		GLCall(int location1 = glGetUniformLocation(shader, "u_VP"));
		if (location1 == -1) {
			ENGINE_LOG("Uniform not found!!!");
		}
		GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(m_Camera_vp)));
	}

	uint32_t RenderingSystem::Draw_Quad(glm::vec2& p0, glm::vec2& p1, glm::vec2& p2, glm::vec2& p3, glm::vec4& color)
	{
		uint32_t vertex_index = vbuffer.Get_Size();

		vbuffer.Fill_Buffer(p0, color, glm::vec2(0.0f, 1.0f), 0);
		vbuffer.Fill_Buffer(p1, color, glm::vec2(1.0f, 1.0f), 0);
		vbuffer.Fill_Buffer(p2, color, glm::vec2(1.0f, 0.0f), 0);
		vbuffer.Fill_Buffer(p3, color, glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	uint32_t RenderingSystem::Draw_Quad(glm::vec2& p0, glm::vec2& p1, glm::vec2& p2, glm::vec2& p3)
	{
		uint32_t vertex_index = vbuffer.Get_Size();

		vbuffer.Fill_Buffer(p0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0);
		vbuffer.Fill_Buffer(p1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0);
		vbuffer.Fill_Buffer(p2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0);
		vbuffer.Fill_Buffer(p3, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	uint32_t RenderingSystem::Draw_Quad(int index)
	{
		uint32_t vertex_index = vbuffer.Get_Size();

		Component::Transform& tr = Globals::Transform.at(index);

		vbuffer.Fill_Buffer({ tr.position.x - tr.scale.x / 2.0f, tr.position.y + tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0);
		vbuffer.Fill_Buffer({ tr.position.x + tr.scale.x / 2.0f, tr.position.y + tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0);
		vbuffer.Fill_Buffer({ tr.position.x + tr.scale.x / 2.0f, tr.position.y - tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0);
		vbuffer.Fill_Buffer({ tr.position.x - tr.scale.x / 2.0f, tr.position.y - tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	void RenderingSystem::Invalidate()
	{
		ENGINE_LOG("Vertex buffer size: %d", vbuffer.Get_Size() / 4);

		ibuffer.Clean();
		ibuffer.Make_Indecies(vbuffer.Get_Size());
		Update_Index_Buffer(vbuffer.Get_Size());
	}

	ShaderProgramSource RenderingSystem::ParseShader(const std::string& filepath)
	{
		std::fstream stream(filepath);

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}
		return { ss[0].str(), ss[1].str() };
	}

	uint32_t RenderingSystem::CompileShader(uint32_t type, const std::string& source)
	{
		GLCall(uint32_t id = glCreateShader(type));
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int lenght;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
			char* message = (char*)alloca(lenght * sizeof(char));
			GLCall(glGetShaderInfoLog(id, lenght, &lenght, message));
			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << "\n";
			std::cout << message << "\n";
			GLCall(glDeleteShader(id));
			return 0;
		}

		return id;
	}

	uint32_t RenderingSystem::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLCall(uint32_t program = glCreateProgram());
		GLCall(uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
		GLCall(uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}


	void RenderingSystem::Initialize(const glm::mat4& vpMatrix)
	{
		FUNCTION_PROFILE();

		vbuffer.total_size = (Globals::entities.size() * 4) + (Globals::Shadow.size() * 12);

		vbuffer.Initialize(vbuffer.total_size);
		ibuffer.Make_Indecies(vbuffer.Get_Size());
		Prepare(vpMatrix);

		Init_Vertex_Buffer();
	}

	/* ------------------------------------------------------------------------ Vertex Buffer --------------------------------------------------------------------------- */

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
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), tr.position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(tr.rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(tr.scale.x, tr.scale.y, 1.0f));

		glm::vec4 quad_vertex_position[4];

		quad_vertex_position[0] = { -0.5f, 0.5f, 0.0f, 1.0f };
		quad_vertex_position[1] = { 0.5f, 0.5f, 0.0f, 1.0f };
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
		//ENGINE_PROFILE("VertexBuffer::Update_Material_On_Quad");

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

	/* ------------------------------------------------------------------- Index Buffer ---------------------------------------------------------------------- */

	void IndexBuffer::Make_Indecies(uint32_t size)
	{
		index_buffer = (uint32_t*)malloc((size / 4) * 6 * sizeof(uint32_t));
		int w = 0;
		for (int k = 0; k < (size / 4) * 6; k += 6) {
			index_buffer[index++] = 0 + w;
			index_buffer[index++] = 3 + w;
			index_buffer[index++] = 2 + w;
			index_buffer[index++] = 2 + w;
			index_buffer[index++] = 1 + w;
			index_buffer[index++] = 0 + w;
			w += 4;
		}

	}

	void IndexBuffer::Clean()
	{
		if (index_buffer != NULL) free(index_buffer);
		index = 0;
	}

	uint32_t* IndexBuffer::Get_Index_Buffer()
	{
		return index_buffer;
	}

}