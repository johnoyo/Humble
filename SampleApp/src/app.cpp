#include "../../Humble/src/Humble.h"
#include "../../Humble/src/Graphics.h"

// User defined entities
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

// User defined components
#include "MyComponents.h"

// User defined systems
#include "Systems\HealthSystem.h"

// User declared systems
HBL::HealthSystem healthSystem;

#include "Levels\Level0.h"
#include "Levels\Level1.h"

int main() 
{
	HBL::Level0 level0(player, camera);
	HBL::Level1 level1(player, camera);

	HBL::Application *app = new HBL::Application(1920.0f, 1080.0f, "SampleApp", false, false, false, 30.0f);

	HBL::Registry::Get().RegisterSystem(&healthSystem);
	HBL::Registry::Get().AddArray<HBL::Component::Health>();

	app->Add_Scene(&level0);
	app->Add_Scene(&level1);

	app->Start();

	delete app;

	return 0;
}