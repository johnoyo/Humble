#pragma once
#include "../Core.h"
#include "../Header.h"
#include "RenderingSystem.h"

namespace HBL {

	class TransformSystem {
	public:
		void Start();
		void Run(VertexBuffer& buffer);
		void Update(VertexBuffer& buffer, std::vector<int>& entities_to_be_updated);
		void Clear();
	private:

	};

}
