#include "TransformSystem.h"

namespace HBL {

	void TransformSystem::Start()
	{
	}

	void TransformSystem::Run(VertexBuffer& buffer)
	{
		//ENGINE_PROFILE("TransformSystem::Run");

		for (uint32_t i = 0; i < Transform.size(); i++) {
			if (Transform.at(i).Static == false) {
				buffer.Update_Position_On_Quad(Transform.at(i).bufferIndex, Transform.at(i));
			}
		}
	}

	void TransformSystem::Clear()
	{
		Transform.clear();
	}

}
