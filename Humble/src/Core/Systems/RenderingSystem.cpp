#include "RenderingSystem.h"
#include "../GlobalSystems.h"

namespace HBL {

	void RenderingSystem::Start()
	{
		FUNCTION_PROFILE();

		const glm::mat4& vpMatrix = GlobalSystems::cameraSystem.Get_View_Projection_Matrix();

		Renderer::Get().AddBatch("res/shaders/Basic.shader", (Globals::s_Registry.GetEntities().size() * 4) + (Globals::s_Registry.GetArray<Component::Shadow>().size() * 12), vpMatrix);

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

		ENGINE_LOG("Transform size: %d", Globals::s_Registry.GetArray<Component::Transform>().size());

		Filter<Component::Transform>().ForEach([&](IEntity& entt) 
		{
			Component::Transform& transform = Globals::s_Registry.GetComponent<Component::Transform>(entt);
			if (transform.Enabled) 
			{
				transform.bufferIndex = Renderer::Get().GetVertexBuffer(0).index;
				Renderer::Get().Draw_Quad(0, transform);
			}
		}).Run();

		Renderer::Get().Bind(0);
		Renderer::Get().Invalidate(0);
		Renderer::Get().UnBind();
	}

}