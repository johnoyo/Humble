#include "TransformSystem.h"
#include "../GlobalSystems.h"

namespace HBL {

	void TransformSystem::Start()
	{
		FUNCTION_PROFILE();

		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		for (auto& component : Registry::Get().GetArray<Component::Transform>())
		{
			Component::Transform& tr = component.second;
			if (tr.Static == false)
			{
				buffer.UpdatePositionOnQuad(tr.bufferIndex, tr);
			}
		}
	}

	void TransformSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		for (auto& component : Registry::Get().GetArray<Component::Transform>())
		{
			Component::Transform& tr = component.second;
			if (tr.Static == false)
			{
				buffer.UpdatePositionOnQuad(tr.bufferIndex, tr);
			}
		}
	}

	void TransformSystem::Clear()
	{
		Clean();
		Registry::Get().GetArray<Component::Transform>().clear();
	}

}
