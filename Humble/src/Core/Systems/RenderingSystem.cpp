#include "RenderingSystem.h"
#include "../GlobalSystems.h"

namespace HBL {

	void RenderingSystem::Start()
	{
		FUNCTION_PROFILE();

		const glm::mat4& vpMatrix = GlobalSystems::cameraSystem.Get_View_Projection_Matrix();

		Renderer::Get().AddBatch("res/shaders/Basic.shader", (Globals::entities.size() * 4) + (Globals::Shadow.size() * 12), vpMatrix);

		Init_Vertex_Buffer();
	}

	void RenderingSystem::Run(float dt)
	{
	}

	void RenderingSystem::Clear()
	{
	}

	void RenderingSystem::Init_Vertex_Buffer()
	{
		Renderer::Get().GetVertexBuffer(0).Reset();

		ENGINE_LOG("Transform size: %d", Globals::Transform.size());

		for (uint32_t i = 0; i < Globals::Transform.size(); i++) {
			if (Globals::Transform.at(i).Enabled) {
				Globals::Transform.at(i).bufferIndex = Renderer::Get().GetVertexBuffer(0).index;
				Renderer::Get().Draw_Quad(0, Globals::Transform.at(i));
			}
		}

		Renderer::Get().Bind(0);
		Renderer::Get().Invalidate(0);
		Renderer::Get().UnBind();
	}

}