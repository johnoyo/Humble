#pragma once
#include "../Renderer.h"
#include "../Header.h"
#include "RenderingSystem.h"

class TransformSystem {
public:
	void Start();
	void Run(VertexBuffer& buffer);
	void Update(VertexBuffer& buffer, std::vector<int>& entities_to_be_updated);
	void Clear();
private:

};
