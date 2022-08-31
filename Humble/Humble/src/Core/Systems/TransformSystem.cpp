#include "TransformSystem.h"
#include "../SystemsHeader.h"

namespace HBL {

	void TransformSystem::Start()
	{
		FUNCTION_PROFILE();

		Filter({ "Transform" });
	}

	void TransformSystem::Run()
	{
		//FUNCTION_PROFILE();

		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		for (int i = 0; i < Globals::Transform.size(); i++)
		{
			Component::Transform& tr = Globals::Transform[i];
			if (tr.Static == false) 
			{
				buffer.Update_Position_On_Quad(tr.bufferIndex, tr);
			}
		}
	}

	void TransformSystem::Clear()
	{
		Clean();
		Globals::Transform.clear();
	}

}
