#include "TransformSystem.h"
#include "../GlobalSystems.h"

namespace HBL {

	void TransformSystem::Start()
	{
		FUNCTION_PROFILE();

		Filter<Component::Transform>();
	}

	void TransformSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();

		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		ForEach([&](IEntity& entt) 
		{
			Component::Transform& tr = Globals::s_Registry.GetComponent<Component::Transform>(entt);
			if (tr.Static == false)
			{
				buffer.UpdatePositionOnQuad(tr.bufferIndex, tr);
			}
		}).Run();
	}

	void TransformSystem::Clear()
	{
		Clean();
		Globals::s_Registry.GetArray<Component::Transform>().clear();
	}

}
