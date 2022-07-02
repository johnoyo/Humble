#include "TransformSystem.h"

namespace HBL {

	void TransformSystem::Start()
	{
	}

	void TransformSystem::Run(VertexBuffer& buffer)
	{
		//ENGINE_PROFILE("TransformSystem::Run");

		for (uint32_t i = 0; i < Globals::Transform.size(); i++) {
			if (Globals::Transform.at(i).Static == false) {
				buffer.Update_Position_On_Quad(Globals::Transform.at(i).bufferIndex, Globals::Transform.at(i));
			}
		}
	}

	void TransformSystem::Clear()
	{
		Globals::Transform.clear();
	}

}
