#include "../../Humble/src/Humble.h"

HBL::IEntity background;
HBL::IEntity player;
HBL::IEntity enemy;
HBL::IEntity wall[400];
HBL::IEntity level[1000];
HBL::IEntity camera;
HBL::IEntity lvlHandler;
HBL::IEntity sps;

#include "Levels\Level0.h"
#include "Levels\Level1.h"

int main() {
	GlobalSystems::windowSystem = HBL::WindowSystem(1920.0f, 1080.0f, "test", false);
	GlobalSystems::cameraSystem = HBL::CameraSystem(0.0f, GlobalSystems::windowSystem.Get_Width(), 0.0f, GlobalSystems::windowSystem.Get_Height());

	HBL::Level0 level0(player);
	HBL::Level1 level1(player);

	HBL::Application *app = new HBL::Application();

	app->Add_Scene(&level0);
	app->Add_Scene(&level1);

	app->Start();

	delete app;

	return 0;
}