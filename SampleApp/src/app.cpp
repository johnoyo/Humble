#include "../../Humble/src/Humble.h"
#include "../../Humble/src/Graphics.h"

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
	HBL::Level0* level0 = new HBL::Level0;
	HBL::Level1* level1 = new HBL::Level1;

	HBL::Application *app = new HBL::Application(1920.0f, 1080.0f, "SampleApp", false, false, false, 30.0f);

	HBL::Registry::Get().RegisterSystem(&healthSystem);
	HBL::Registry::Get().AddArray<HBL::Component::Health>();

	app->AddScene(level0);
	app->AddScene(level1);

	app->Start();

	delete app;

	return 0;
}