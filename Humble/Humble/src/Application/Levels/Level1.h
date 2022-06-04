#pragma once

#include "SystemsHeader.h"
#include "..\ecs\Scene.h"

namespace HBL {

	class Level1 : public Scene
	{
	public:
		void Enroll() override
		{
		}
		void Add() override
		{
		}
		void Init() override
		{
		}

		void Update() override
		{
			scriptingSystem.Run(LevelManager::GetCurrentLevel());
			textureSystem.Run(renderingSystem.Get_Vertex_Buffer());
			gravitySystem.Run();
			transformSystem.Run(renderingSystem.Get_Vertex_Buffer());
			collisionSystem.Run(renderingSystem.Get_Vertex_Buffer());
			shadowSystem.Run(GET_COMPONENT(Transform, player).position, renderingSystem.Get_Vertex_Buffer(), renderingSystem);
		}
	};

}