#pragma once
#include "../HumbleAPI.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "../Core.h"
#include "../Utilities/OpenGLDebug.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace HBL {

	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	struct RendererData
	{
		VertexBuffer vbuffer;
		IndexBuffer ibuffer;
		ShaderProgramSource sp;

		uint32_t vao = 0;
		uint32_t vb = 0;
		uint32_t ib = 0;
		uint32_t shader = 0;
	};

	class HBL_API Renderer {
	public:
		Renderer(const Renderer&) = delete;

		static Renderer& Get()
		{
			static Renderer instance;
			return instance;
		}

		void AddBatch(const std::string& shader_path, uint32_t vertexBufferSize, const glm::mat4& m_Camera_vp);
		void Render(const glm::mat4& camera_vp);

		void Bind(uint32_t index);
		void Bind();
		void UnBind();

		uint32_t Draw_Quad(uint32_t vindex, glm::vec2& p0, glm::vec2& p1, glm::vec2& p2, glm::vec2& p3, glm::vec4& color);
		uint32_t Draw_Quad(uint32_t vindex, glm::vec2& p0, glm::vec2& p1, glm::vec2& p2, glm::vec2& p3);
		uint32_t Draw_Quad(uint32_t vindex, Component::Transform& tr);
		uint32_t Draw_Quad(uint32_t vindex, Component::TextTransform& tr, float width, float height);

		void Invalidate(uint32_t vindex);

		VertexBuffer& GetVertexBuffer(uint32_t index) { return rendererData[index]->vbuffer; }
		IndexBuffer& GetIndexBuffer(uint32_t index) { return rendererData[index]->ibuffer; }

		void Clear();
		void Clear_Buffers();

	private:
		Renderer() {}

		void BeginFrame();
		void EndFrame();

		void Prepare(const glm::mat4& m_Camera_vp, const std::string& shader_path, bool debug = true);

		void UpdateIndexBuffer(uint32_t size, uint32_t vindex);
		void UpdateCameraUniform(const glm::mat4& m_Camera_vp);

		ShaderProgramSource ParseShader(const std::string& filepath);
		uint32_t CompileShader(uint32_t type, const std::string& source);
		uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

		std::vector<RendererData*> rendererData;
		uint32_t size = 0;
		uint32_t currentIndex = 0;
	};

}