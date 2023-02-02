#include "TransformSystem.h"

namespace HBL 
{
	void TransformSystem::Start()
	{
		FUNCTION_PROFILE();

		Registry::Get().View<Component::Transform>().ForEach([&](Component::Transform& tr)
		{
			if (tr.Static == false)
			{
				Renderer::Get().UpdateQuad(0, tr.bufferIndex, tr.position, tr.rotation, tr.scale);
			}
		}).Run();
	}

	void TransformSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		Registry::Get().View<Component::Transform>().ForEach([&](Component::Transform& tr)
		{
			if (tr.Static == false)
			{
				Renderer::Get().UpdateQuad(0, tr.bufferIndex, tr.position, tr.rotation, tr.scale);
			}
		}).Run(); 
	}

	void TransformSystem::Clear()
	{
		Registry::Get().ClearArray<Component::Transform>();
	}
}
