#include "../../Humble/src/Humble.h"
#include "../../Humble/src/Graphics.h"


// User defined entities
HBL::IEntity background;
HBL::IEntity player;
HBL::IEntity enemy;
HBL::IEntity wall[400];
HBL::IEntity level[1000];
HBL::IEntity camera;
HBL::IEntity lvlHandler;
HBL::IEntity sps;

// User defined components
#include "MyComponents.h"

// User defined component vecctors
std::vector<HBL::Component::Health> Health;

// User defined systems
#include "Systems\HealthSystem.h"

// User declared systems
HBL::HealthSystem healthSystem;

#include "Levels\Level0.h"
#include "Levels\Level1.h"

int main() 
{
	HBL::Level0 level0(player);
	HBL::Level1 level1(player);

	HBL::Application *app = new HBL::Application(1920.0f, 1080.0f, "SampleApp", false);

	app->Register_System(&healthSystem);

	app->Add_Scene(&level0);
	app->Add_Scene(&level1);

	app->Start();

	delete app;

	return 0;
}