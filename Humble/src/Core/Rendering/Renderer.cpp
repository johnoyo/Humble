#include "Renderer.h"

namespace HBL 
{
	void Renderer::AddBatch(const std::string& shaderPath, uint32_t vertexBufferSize, IEntity& camera)
	{
		glm::mat4& cameraVP = Registry::Get().GetComponent<Component::Camera>(camera).viewProjectionMatrix;

		rendererData.push_back(new RendererData());

		rendererData[size]->vbuffer.total_size = vertexBufferSize;
		rendererData[size]->vbuffer.Initialize(vertexBufferSize);
		rendererData[size]->ibuffer.MakeIndecies(rendererData[size]->vbuffer.GetSize());

		Prepare(cameraVP, shaderPath);

		rendererData[size]->vbuffer.Reset();

		UnBind();

		size++;
	}

	void Renderer::Render(IEntity& camera)
	{
		glm::mat4& cameraVP = Registry::Get().GetComponent<Component::Camera>(camera).viewProjectionMatrix;

		BeginFrame();

		for (RendererData* data : rendererData)
		{
			Bind();

			UpdateCameraUniform(cameraVP);

			// Set dynamic vertex buffer
			glBufferSubData(GL_ARRAY_BUFFER, 0, data->vbuffer.GetSize() * sizeof(Vertex_Array), data->vbuffer.GetBuffer());

			// Draw batch
			glDrawElements(GL_TRIANGLES, (data->vbuffer.GetSize() / 4) * 6, GL_UNSIGNED_INT, NULL);

			UnBind();

			currentIndex++;
		}

		EndFrame();
	}

	void Renderer::Bind(uint32_t index)
	{
		glBindVertexArray(rendererData[index]->vao);
		glBindBuffer(GL_ARRAY_BUFFER, rendererData[index]->vb);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererData[index]->ib);
		glUseProgram(rendererData[index]->shader);
	}

	void Renderer::Bind()
	{
		glBindVertexArray(rendererData[currentIndex]->vao);
		glBindBuffer(GL_ARRAY_BUFFER, rendererData[currentIndex]->vb);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererData[currentIndex]->ib);
		glUseProgram(rendererData[currentIndex]->shader);
	}

	void Renderer::UnBind()
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}



	uint32_t Renderer::RegisterQuad(uint32_t vindex, glm::vec2& p0, glm::vec2& p1, glm::vec2& p2, glm::vec2& p3, glm::vec4& color)
	{
		uint32_t vertex_index = rendererData[vindex]->vbuffer.GetSize();

		rendererData[vindex]->vbuffer.FillBuffer(p0, color, glm::vec2(0.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer(p1, color, glm::vec2(1.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer(p2, color, glm::vec2(1.0f, 0.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer(p3, color, glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	uint32_t Renderer::RegisterQuad(uint32_t vindex, glm::vec2& p0, glm::vec2& p1, glm::vec2& p2, glm::vec2& p3)
	{
		uint32_t vertex_index = rendererData[vindex]->vbuffer.GetSize();

		rendererData[vindex]->vbuffer.FillBuffer(p0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer(p1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer(p2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer(p3, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	uint32_t Renderer::RegisterQuad(uint32_t vindex, Component::Transform& tr)
	{
		uint32_t vertex_index = rendererData[vindex]->vbuffer.GetSize();

		rendererData[vindex]->vbuffer.FillBuffer({ tr.position.x - tr.scale.x / 2.0f, tr.position.y + tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer({ tr.position.x + tr.scale.x / 2.0f, tr.position.y + tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer({ tr.position.x + tr.scale.x / 2.0f, tr.position.y - tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer({ tr.position.x - tr.scale.x / 2.0f, tr.position.y - tr.scale.y / 2.0f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	uint32_t Renderer::RegisterQuad(uint32_t vindex, Component::TextTransform& tr, float width, float height)
	{
		uint32_t vertex_index = rendererData[vindex]->vbuffer.GetSize();

		rendererData[vindex]->vbuffer.FillBuffer({ tr.position.x - (tr.scale.x / 2.0f * width), tr.position.y + (tr.scale.y / 2.0f * height) }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer({ tr.position.x + (tr.scale.x / 2.0f * width), tr.position.y + (tr.scale.y / 2.0f * height) }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer({ tr.position.x + (tr.scale.x / 2.0f * width), tr.position.y - (tr.scale.y / 2.0f * height) }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), 0);
		rendererData[vindex]->vbuffer.FillBuffer({ tr.position.x - (tr.scale.x / 2.0f * width), tr.position.y - (tr.scale.y / 2.0f * height) }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), 0);

		return vertex_index;
	}

	void Renderer::UpdateQuad(uint32_t batchIndex, int vertexIndex, glm::vec3& position, float rotation, glm::vec3& scale)
	{
		if (vertexIndex == -1)
			return;

		VertexBuffer& buffer = rendererData[batchIndex]->vbuffer;
		buffer.UpdatePositionOnQuad(vertexIndex, position, rotation, scale);
	}

	void HBL::Renderer::UpdateQuad(uint32_t batchIndex, int vertexIndex, glm::vec4& color, uint32_t textureIndex)
	{
		if (vertexIndex == -1)
			return;

		VertexBuffer& buffer = rendererData[batchIndex]->vbuffer;
		buffer.UpdateMaterialOnQuad(vertexIndex, color, textureIndex);
	}

	void HBL::Renderer::UpdateQuad(uint32_t batchIndex, int vertexIndex, glm::vec4& color, float textureIndex, glm::vec2& texCoords, glm::vec2& sheetSize, glm::vec2& spriteSize)
	{
		if (vertexIndex == -1)
			return;

		VertexBuffer& buffer = rendererData[batchIndex]->vbuffer;
		buffer.UpdateMaterialOnQuad(vertexIndex, color, textureIndex, texCoords, sheetSize, spriteSize);
	}

	void HBL::Renderer::UpdateQuad(uint32_t batchIndex, int vertexIndex, glm::vec3& position, float rotation, glm::vec3& scale, glm::vec4& color, uint32_t textureIndex)
	{
		if (vertexIndex == -1)
			return;

		VertexBuffer& buffer = rendererData[batchIndex]->vbuffer;
		buffer.UpdateMaterialOnQuad(vertexIndex, color, textureIndex);
	}

	void Renderer::Invalidate(uint32_t vindex)
	{
		ENGINE_LOG("Vertex buffer size: %d", rendererData[vindex]->vbuffer.GetSize() / 4);

		rendererData[vindex]->ibuffer.Clean();
		rendererData[vindex]->ibuffer.MakeIndecies(rendererData[vindex]->vbuffer.GetSize());
		UpdateIndexBuffer(rendererData[vindex]->vbuffer.GetSize(), vindex);
	}

	void Renderer::Clear()
	{
		FUNCTION_PROFILE();

		for (RendererData* data : rendererData)
		{
			glDeleteBuffers(1, &data->vb);
			glDeleteBuffers(1, &data->ib);
			glDeleteProgram(data->shader);
			glDeleteVertexArrays(1, &data->vao);

			data->vbuffer.Clean();
			data->ibuffer.Clean();

			delete data;
		}

		size = 0;
		rendererData.clear();
	}

	void Renderer::ClearBuffers()
	{
		for (RendererData* data : rendererData)
		{
			data->vbuffer.Clean();
			data->ibuffer.Clean();

			delete data;
		}

		size = 0;
		rendererData.clear();
	}

	void HBL::Renderer::BeginFrame()
	{
		currentIndex = 0;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void HBL::Renderer::EndFrame()
	{
	}

	void Renderer::Prepare(const glm::mat4& cameraVP, const std::string& shaderPath, bool debug)
	{
		FUNCTION_PROFILE();

		if (debug)
			GLDebug::EnableGLDebugging();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/* vertex array object */
		glGenVertexArrays(1, &rendererData[size]->vao);
		glBindVertexArray(rendererData[size]->vao);

		/* vertex buffer */
		glGenBuffers(1, &rendererData[size]->vb);
		glBindBuffer(GL_ARRAY_BUFFER, rendererData[size]->vb);
		glBufferData(GL_ARRAY_BUFFER, rendererData[size]->vbuffer.GetTotalSize() * sizeof(struct Vertex_Array), nullptr, GL_DYNAMIC_DRAW);

		/* vertex attrib positions*/
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, position));

		/* vertex attrib colors*/
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, color));

		/* vertex attrib texture coordinates*/
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, tex_coord));

		/* vertex attrib texture id*/
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(struct Vertex_Array), (const void*)offsetof(Vertex_Array, tex_id));

		/* index buffer */
		glGenBuffers(1, &rendererData[size]->ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererData[size]->ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rendererData[size]->vbuffer.GetSize() / 4) * 6 * sizeof(uint32_t), rendererData[size]->ibuffer.GetIndexBuffer(), GL_STATIC_DRAW);

		/* shaders */
		ShaderProgramSource shaderSource = ParseShader(shaderPath);
		rendererData[size]->shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
		glUseProgram(rendererData[size]->shader);

		auto loc = glGetUniformLocation(rendererData[size]->shader, "u_textures");
		int samplers[32];
		for (uint32_t i = 0; i < 32; i++)
			samplers[i] = i;
		glUniform1iv(loc, 32, samplers);

		auto location1 = glGetUniformLocation(rendererData[size]->shader, "u_VP");
		if (location1 == -1) {
			ENGINE_LOG("Uniform not found!!!");
		}
		glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(cameraVP));
	}

	void Renderer::UpdateIndexBuffer(uint32_t size, uint32_t vindex)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size / 4) * 6 * sizeof(uint32_t), rendererData[vindex]->ibuffer.GetIndexBuffer(), GL_STATIC_DRAW);
	}

	void Renderer::UpdateCameraUniform(const glm::mat4& cameraVP)
	{
		auto location1 = glGetUniformLocation(rendererData[currentIndex]->shader, "u_VP");
		if (location1 == -1) {
			ENGINE_LOG("Uniform not found!!!");
		}
		glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(cameraVP));
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
		uint32_t id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int lenght;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
			char* message = (char*)alloca(lenght * sizeof(char));
			glGetShaderInfoLog(id, lenght, &lenght, message);
			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << "\n";
			std::cout << message << "\n";
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	uint32_t Renderer::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		uint32_t program = glCreateProgram();
		uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}
}
