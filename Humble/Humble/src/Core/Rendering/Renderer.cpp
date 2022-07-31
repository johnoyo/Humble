#include "Renderer.h"

namespace HBL {

	void Renderer::AddBatch(const std::string& shader_path, uint32_t vertexBufferSize, const glm::mat4& m_Camera_vp)
	{
		rendererData.push_back(new RendererData());

		rendererData[size]->vbuffer.total_size = vertexBufferSize;
		rendererData[size]->vbuffer.Initialize(vertexBufferSize);
		rendererData[size]->ibuffer.Make_Indecies(rendererData[size]->vbuffer.Get_Size());

		Prepare(m_Camera_vp, shader_path);

		rendererData[size]->vbuffer.Reset();

		size++;
	}

	void Renderer::Render(const glm::mat4& m_Camera_vp)
	{
		currentIndex = 0;

		for (RendererData* data : rendererData)
		{
			Bind();

			UpdateCameraUniform(m_Camera_vp);

			/* set dynamic vertex buffer */
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, data->vb));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, data->vbuffer.Get_Size() * sizeof(Vertex_Array), data->vbuffer.Get_Buffer()));

			/* Render here */
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			GLCall(glDrawElements(GL_TRIANGLES, (data->vbuffer.Get_Size() / 4) * 6, GL_UNSIGNED_INT, NULL));

			UnBind();

			currentIndex++;
		}
	}

	void Renderer::Bind()
	{
		GLCall(glBindVertexArray(rendererData[currentIndex]->vao));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererData[currentIndex]->vb));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererData[currentIndex]->ib));
		GLCall(glUseProgram(rendererData[currentIndex]->shader));
	}

	void Renderer::UnBind()
	{
		GLCall(glBindVertexArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCall(glUseProgram(0));
	}

	uint32_t Renderer::Draw_Quad(uint32_t vindex, glm::vec2& p0, glm::vec2& p1, glm::vec2& p2, glm::vec2& p3, glm::vec4& color)
	{
		uint32_t vertex_index = rendererData[vindex]->vbuffer.Get_Size();

		rendererData[vindex]->vbuffer.Fill_Buffer(p0, color, glm::vec2(0.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.Fill_Buffer(p1, color, glm::vec2(1.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.Fill_Buffer(p2, color, glm::vec2(1.0f, 0.0f), 0);
		rendererData[vindex]->vbuffer.Fill_Buffer(p3, color, glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	uint32_t Renderer::Draw_Quad(uint32_t vindex, glm::vec2& p0, glm::vec2& p1, glm::vec2& p2, glm::vec2& p3)
	{
		uint32_t vertex_index = rendererData[vindex]->vbuffer.Get_Size();

		rendererData[vindex]->vbuffer.Fill_Buffer(p0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.Fill_Buffer(p1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.Fill_Buffer(p2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0);
		rendererData[vindex]->vbuffer.Fill_Buffer(p3, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	uint32_t Renderer::Draw_Quad(uint32_t vindex, int index)
	{
		uint32_t vertex_index = rendererData[vindex]->vbuffer.Get_Size();

		Component::Transform& tr = Globals::Transform.at(index);

		rendererData[vindex]->vbuffer.Fill_Buffer({ tr.position.x - tr.scale.x / 2.0f, tr.position.y + tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.Fill_Buffer({ tr.position.x + tr.scale.x / 2.0f, tr.position.y + tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.Fill_Buffer({ tr.position.x + tr.scale.x / 2.0f, tr.position.y - tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0);
		rendererData[vindex]->vbuffer.Fill_Buffer({ tr.position.x - tr.scale.x / 2.0f, tr.position.y - tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	void Renderer::Invalidate(uint32_t vindex)
	{
		ENGINE_LOG("Vertex buffer size: %d", rendererData[vindex]->vbuffer.Get_Size() / 4);

		rendererData[vindex]->ibuffer.Clean();
		rendererData[vindex]->ibuffer.Make_Indecies(rendererData[vindex]->vbuffer.Get_Size());
		UpdateIndexBuffer(rendererData[vindex]->vbuffer.Get_Size());
	}

	void Renderer::Clear()
	{
		FUNCTION_PROFILE();

		for (RendererData* data : rendererData)
		{
			GLCall(glDeleteBuffers(1, &data->vb));
			GLCall(glDeleteBuffers(1, &data->ib));
			GLCall(glDeleteProgram(data->shader));
			GLCall(glDeleteVertexArrays(1, &data->vao));

			data->vbuffer.Clean();
			data->ibuffer.Clean();
		}
	}

	void Renderer::Clear_Buffers()
	{
		for (RendererData* data : rendererData)
		{
			data->vbuffer.Clean();
			data->ibuffer.Clean();
		}
	}

	void Renderer::Prepare(const glm::mat4& m_Camera_vp, const std::string& shader_path)
	{
		FUNCTION_PROFILE();

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* vertex array object */
		GLCall(glGenVertexArrays(1, &rendererData[size]->vao));
		GLCall(glBindVertexArray(rendererData[size]->vao));

		/* vertex buffer */
		GLCall(glGenBuffers(1, &rendererData[size]->vb));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererData[size]->vb));
		GLCall(glBufferData(GL_ARRAY_BUFFER, rendererData[size]->vbuffer.Get_Total_Size() * sizeof(struct Vertex_Array), nullptr, GL_DYNAMIC_DRAW));

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
		GLCall(glGenBuffers(1, &rendererData[size]->ib));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererData[size]->ib));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rendererData[size]->vbuffer.Get_Size() / 4) * 6 * sizeof(uint32_t), rendererData[size]->ibuffer.Get_Index_Buffer(), GL_STATIC_DRAW));

		/* shaders */
		ShaderProgramSource shaderSource = ParseShader(shader_path);
		rendererData[size]->shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
		GLCall(glUseProgram(rendererData[size]->shader));

		GLCall(auto loc = glGetUniformLocation(rendererData[size]->shader, "u_textures"));
		int samplers[32];
		for (uint32_t i = 0; i < 32; i++)
			samplers[i] = i;
		GLCall(glUniform1iv(loc, 32, samplers));

		GLCall(auto location1 = glGetUniformLocation(rendererData[size]->shader, "u_VP"));
		if (location1 == -1) {
			ENGINE_LOG("Uniform not found!!!");
		}
		GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(m_Camera_vp)));
	}

	void Renderer::UpdateIndexBuffer(uint32_t size)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size / 4) * 6 * sizeof(uint32_t), rendererData[currentIndex]->ibuffer.Get_Index_Buffer(), GL_STATIC_DRAW));
	}

	void Renderer::UpdateCameraUniform(const glm::mat4& m_Camera_vp)
	{
		GLCall(auto location1 = glGetUniformLocation(rendererData[currentIndex]->shader, "u_VP"));
		if (location1 == -1) {
			ENGINE_LOG("Uniform not found!!!");
		}
		GLCall(glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(m_Camera_vp)));
	}

	ShaderProgramSource Renderer::ParseShader(const std::string& filepath)
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

	uint32_t Renderer::CompileShader(uint32_t type, const std::string& source)
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

	uint32_t Renderer::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

}
