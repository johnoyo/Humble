#include "../../Humble/src/Humble.h"
#include "../../Humble/src/Graphics.h"

// User defined entities
HBL::IEntity background;
HBL::IEntity player;
HBL::IEntity enemy;
HBL::IEntity wall[400];
HBL::IEntity level[10000];
HBL::IEntity camera;
HBL::IEntity lvlHandler;
HBL::IEntity sps;
HBL::IEntity text;
HBL::IEntity FPSCounter;

// User defined components
#include "MyComponents.h"

// User defined systems
#include "Systems\HealthSystem.h"

// User declared systems
HBL::HealthSystem healthSystem;

#include "Levels\Level0.h"
#include "Levels\Level1.h"
#include <typeinfo>

int main() 
{
	/*{
		HBL::System system;

		for (uint32_t i = 0; i < 100000; i++)
		{
			system.EnrollEntity(HBL::IEntity(), Globals::entities);
		}

		for (uint32_t i = 0; i < 100000; i++)
		{
			system.AddComponent<HBL::Component::Material>(Globals::entities[i], "Material", Globals::entities, Globals::Material);
			system.AddComponent<HBL::Component::Transform>(Globals::entities[i], "Transform", Globals::entities, Globals::Transform);
		}

		auto T = system.GetComponent<HBL::Component::Material>(Globals::entities[3485], "Material", Globals::Material);
	}*/

	//HBL::Random::Init();
	//
	//Globals::s_Registry.AddArray<HBL::Component::TextTransform>();
	//Globals::s_Registry.AddArray<HBL::Component::Transform>();
	//Globals::s_Registry.AddArray<HBL::Component::Material>();
	//Globals::s_Registry.AddArray<HBL::Component::Animation>();
	//Globals::s_Registry.AddArray<HBL::Component::AudioSource>();
	//Globals::s_Registry.AddArray<HBL::Component::Gravity>();
	//Globals::s_Registry.AddArray<HBL::Component::Health>();
	//Globals::s_Registry.AddArray<HBL::Component::Script>();
	//Globals::s_Registry.AddArray<HBL::Component::Shadow>();
	//Globals::s_Registry.AddArray<HBL::Component::CollisionBox>();

	//for (uint32_t i = 0; i < 100000; i++)
	//{
	//	Globals::s_Registry.EnrollEntity(HBL::IEntity());
	//}

	//for (uint32_t i = 0; i < 100000; i++)
	//{
	//	Globals::s_Registry.AddComponent<HBL::Component::Material>(Globals::s_Registry.GetEntities()[i]);
	//	Globals::s_Registry.AddComponent<HBL::Component::Transform>(Globals::s_Registry.GetEntities()[i]);
	//}

	//auto T = Globals::s_Registry.GetComponent<HBL::Component::Transform>(Globals::s_Registry.GetEntities()[3485]);

	HBL::Level0 level0(player, camera);
	HBL::Level1 level1(player, camera);

	HBL::Application *app = new HBL::Application(1920.0f, 1080.0f, "SampleApp", false, false, false, 30.0f);

	Globals::s_Registry.RegisterSystem(&healthSystem);
	Globals::s_Registry.AddArray<HBL::Component::Health>();

	app->Add_Scene(&level0);
	app->Add_Scene(&level1);

	app->Start();

	delete app;

	return 0;
}