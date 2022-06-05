#include "..\ecs\Header.h"
#include "..\ecs\Utilities.h"
#include "..\ecs\Application.h"
#include "..\ecs\Scene.h"
#include "Levels/Level0.h"
#include "Levels/Level1.h"

System ecs = System();

std::vector<Entity::BaseEntity> entities;

Entity::BaseEntity background;
Entity::BaseEntity player;
Entity::BaseEntity enemy;
Entity::BaseEntity wall[400];
Entity::BaseEntity level[10000];
Entity::BaseEntity camera;
Entity::BaseEntity lvlHandler;
Entity::BaseEntity sps;

std::vector<Component::Transform> Transform;
std::vector<Component::CollisionBox> CollisionBox;
std::vector<Component::Material> Material;
std::vector<Component::Health> Health;
std::vector<Component::Script> Script;
std::vector<Component::Gravity> Gravity;
std::vector<Component::Shadow> Shadow;

static WindowSystem windowSystem = WindowSystem(1920.0f, 1080.0f, "test", false);
static TextureSystem textureSystem;
static RenderingSystem renderingSystem;
static CameraSystem cameraSystem = CameraSystem(0.0f, windowSystem.Get_Width(), 0.0f, windowSystem.Get_Height());
static ScriptingSystem scriptingSystem;
static CollisionSystem collisionSystem;
static GravitySystem gravitySystem;
static TransformSystem transformSystem;
static ShadowCastSystem shadowSystem;

int main() {
	HBL::Level0 level0;
	HBL::Level1 level1;

	HBL::Application *app = new HBL::Application();
	app->Add_Scene(&level0);
	app->Add_Scene(&level1);

	app->Start();

	delete app;

	return 0;
}