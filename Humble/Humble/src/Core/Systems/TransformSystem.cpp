#include "TransformSystem.h"
#include "../SystemsHeader.h"

namespace HBL {

	void TransformSystem::Start()
	{
		FUNCTION_PROFILE();

		Filter(Globals::entities, "Transform");
	}

	void TransformSystem::Run()
	{
		//FUNCTION_PROFILE();
		VertexBuffer& buffer = GlobalSystems::renderingSystem.Get_Vertex_Buffer();

		For_Each([&](IEntity& entt)
		{
			Component::Transform& tr = GET_COMPONENT(Transform, entt);
			if (tr.Static == false) {
				buffer.Update_Position_On_Quad(tr.bufferIndex, tr);
			}
		});
	}

	void TransformSystem::Clear()
	{
		Globals::Transform.clear();
	}

}
