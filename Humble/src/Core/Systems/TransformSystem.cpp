#include "TransformSystem.h"

namespace HBL 
{
	void TransformSystem::Start()
	{
		FUNCTION_PROFILE();

		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		View<Component::Transform>().ForEach([&](Component::Transform& tr)
		{
			if (tr.Static == false)
			{
				if (tr.bufferIndex != -1)
					buffer.UpdatePositionOnQuad(tr.bufferIndex, tr);
			}
		}).Run();
	}

	void TransformSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		View<Component::Transform>().ForEach([&](Component::Transform& tr)
		{
			if (tr.Static == false)
			{
				if (tr.bufferIndex != -1)
					buffer.UpdatePositionOnQuad(tr.bufferIndex, tr);
			}
		}).Run(); 
	}

	void TransformSystem::Clear()
	{
		Registry::Get().ClearArray<Component::Transform>();
	}
}
