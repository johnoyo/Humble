#include "..\Core\Header.h"
#include "..\Core\Utilities.h"
#include "GlobalsHeader.h"

#include "..\Core\Application.h"
#include "..\Core\Scene.h"

#include "Levels\Level0.h"
#include "Levels\Level1.h"

HBL::System ecs = HBL::System();

std::vector<HBL::IEntity> entities;

HBL::IEntity background;
HBL::IEntity player;
HBL::IEntity enemy;
HBL::IEntity wall[400];
HBL::IEntity level[10000];
HBL::IEntity camera;
HBL::IEntity lvlHandler;
HBL::IEntity sps;

std::vector<HBL::Component::Transform> Transform;
std::vector<HBL::Component::CollisionBox> CollisionBox;
std::vector<HBL::Component::Material> Material;
std::vector<HBL::Component::Health> Health;
std::vector<HBL::Component::Script> Script;
std::vector<HBL::Component::Gravity> Gravity;
std::vector<HBL::Component::Shadow> Shadow;

static HBL::WindowSystem windowSystem = HBL::WindowSystem(1920.0f, 1080.0f, "test", false);
static HBL::TextureSystem textureSystem;
static HBL::RenderingSystem renderingSystem;
static HBL::CameraSystem cameraSystem = HBL::CameraSystem(0.0f, windowSystem.Get_Width(), 0.0f, windowSystem.Get_Height());
static HBL::ScriptingSystem scriptingSystem;
static HBL::CollisionSystem collisionSystem;
static HBL::GravitySystem gravitySystem;
static HBL::TransformSystem transformSystem;
static HBL::ShadowCastSystem shadowSystem;


int main() {

	HBL::Level0 level0(player);
	HBL::Level1 level1(player);

	HBL::Application *app = new HBL::Application();

	app->Add_Scene(&level0);
	app->Add_Scene(&level1);

	app->Start();

	delete app;

	return 0;
}