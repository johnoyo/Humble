#pragma once

#include "Humble.h"
#include "..\MyComponents.h"

#include "..\Scripts\PlayerScript.h"
#include "..\Scripts\EnemyScript.h"
#include "..\Scripts\LevelHandlerScript.h"

namespace HBL 
{
	class Level1 final : public IScene
	{
	public:
		virtual void OnAttach() override;
		virtual void OnCreate() override;
		virtual void OnDetach() override;

		void ILoadLevel(const std::string& level_path);

	private:
		HBL::IEntity background;
		HBL::IEntity player;
		HBL::IEntity enemy;
		HBL::IEntity wall[400];
		HBL::IEntity level[10000];
		HBL::IEntity camera;
		HBL::IEntity clipCamera;
		HBL::IEntity lvlHandler;
		HBL::IEntity sps;
		HBL::IEntity text;
		HBL::IEntity FPSCounter;
	};
}