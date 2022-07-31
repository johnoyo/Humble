#pragma once

#include "../Core.h"
#include "../Header.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../Rendering/Renderer.h"
#include "../ISystem.h"

namespace HBL {

	class HBL_API RenderingSystem final : public ISystem {
	public:

		virtual void Start() override;
		virtual void Run() override;
		virtual void Clear() override;

		void Init_Vertex_Buffer();
	};

}
