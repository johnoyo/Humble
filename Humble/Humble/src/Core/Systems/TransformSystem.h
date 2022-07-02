#pragma once
#include "../Core.h"
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "RenderingSystem.h"

namespace HBL {

	class HBL_API TransformSystem {
	public:
		void Start();
		void Run(VertexBuffer& buffer);
		void Clear();
	};

}
