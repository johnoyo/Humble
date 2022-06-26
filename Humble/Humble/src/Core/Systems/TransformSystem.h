#pragma once
#include "../Core.h"
#include "../Header.h"
#include "RenderingSystem.h"

namespace HBL {

	class TransformSystem {
	public:
		void Start();
		void Run(VertexBuffer& buffer);
		void Clear();
	};

}
