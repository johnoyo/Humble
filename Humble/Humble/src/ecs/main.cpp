#include "Header.h"
#include<iostream>

#include "Systems\CameraSystem.h"
#include "Systems\RenderingSystem.h"
#include "Systems\WindowSystem.h"
#include "Systems\TextureSystem.h"
#include "Systems\ScriptingSystem.h"
#include "Systems\InputSystem.h"
#include "Systems\CollisionSystem.h"
#include "Systems\GravitySystem.h"
#include "Systems\LevelSystem.h"
#include "Systems\SoundSystem.h"
#include "Systems\TransformSystem.h"
#include "Systems\ShadowCastSystem.h"

System ecs = System();

std::vector<Entity::BaseEntity> entities;

/* ------------------------------- Entities ---------------------------- */
Entity::BaseEntity background;
Entity::BaseEntity player;
Entity::BaseEntity enemy;
Entity::BaseEntity wall[400];
Entity::BaseEntity level[10000];
Entity::BaseEntity camera;
Entity::BaseEntity lvlHandler;
Entity::BaseEntity sps;
/* --------------------------------------------------------------------- */

/* ------------------------- Component Vectors -------------------------- */
std::vector<Component::Transform> Transform;
std::vector<Component::CollisionBox> CollisionBox;
std::vector<Component::Material> Material;
std::vector<Component::Health> Health;
std::vector<Component::Script> Script;
std::vector<Component::Gravity> Gravity;
std::vector<Component::Shadow> Shadow;
/* --------------------------------------------------------------------- */

/* ------------------------------------ Systems ------------------------------------ */
static WindowSystem windowSystem = WindowSystem(1920.0f, 1080.0f, "test", false);
static TextureSystem textureSystem;
static RenderingSystem renderingSystem;
static CameraSystem cameraSystem = CameraSystem(0.0f, windowSystem.Get_Width(), 0.0f, windowSystem.Get_Height());
static ScriptingSystem scriptingSystem;
static InputSystem inputSystem;
static CollisionSystem collisionSystem;
static GravitySystem gravitySystem;
static LevelSystem levelSystem;
static SoundSystem soundSystem;
static TransformSystem transformSystem;
static ShadowCastSystem shadowSystem;
/* --------------------------------------------------------------------------------- */

#include "EntitiesCode\Player.h"
#include "EntitiesCode\Enemy.h"
#include "EntitiesCode\LevelHandler.h"

#include "Utilities.h"

/* Get rid of the console when the application is running on release mode */
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define WIN32

#ifdef WIN32
#include <windows.h>
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif //def WIN32


int main() {
/* ------------------------------------ Enroll Entities ------------------------------------ */
	ENROLL_ENTITY(background);
	ENROLL_ENTITY(player);
	ENROLL_ENTITY(enemy);
	ENROLL_ENTITY(camera);
	ENROLL_ENTITY(lvlHandler);
	ENROLL_ENTITY(sps);

	for (uint32_t i = 0; i < 400; i++)
		ENROLL_ENTITY(wall[i]);

	for (uint32_t i = 0; i < 10000; i++)
		ENROLL_ENTITY(level[i]);

/* ----------------------------------------------------------------------------------------- */
	ENGINE_LOG("Entities size: %d", entities.size());
/* ------------------------------------ Add Components to Entities ------------------------------------ */
	ADD_COMPONENT(Transform, background);
	ADD_COMPONENT(Material, background);

	ADD_COMPONENT(Transform, player);
	ADD_COMPONENT(Health, player);
	ADD_COMPONENT(Script, player);
	ADD_COMPONENT(Material, player);
	ADD_COMPONENT(CollisionBox, player);
	//ADD_COMPONENT(Gravity, player);

	ADD_COMPONENT(Transform, enemy);
	ADD_COMPONENT(Script, enemy);
	ADD_COMPONENT(Material, enemy);
	ADD_COMPONENT(CollisionBox, enemy);
	ADD_COMPONENT(Gravity, enemy);
	ADD_COMPONENT(Shadow, enemy);

	ADD_COMPONENT(Script, lvlHandler);
	
	ADD_COMPONENT(Transform, camera);

	ADD_COMPONENT(Material, sps);
	
	for (uint32_t i = 0; i < 400; i++) {
		ADD_COMPONENT(Transform, wall[i]);
		ADD_COMPONENT(CollisionBox, wall[i]);
		ADD_COMPONENT(Material, wall[i]);
		//ADD_COMPONENT(Shadow, wall[i]);
	}

	for (uint32_t i = 0; i < 10000; i++) {
		ADD_COMPONENT(Transform, level[i]);
		ADD_COMPONENT(CollisionBox, level[i]);
		ADD_COMPONENT(Material, level[i]);
	}

	ENGINE_LOG("Materials size: %d", Material.size());
/* ---------------------------------------------------------------------------------------------------- */

/* ------------------------------------ Attach scripts to Entities ------------------------------------ */
	GET_COMPONENT(Script, player).init.push_back(Player::Level_0::init);
	GET_COMPONENT(Script, player).update.push_back(Player::Level_0::update);

	GET_COMPONENT(Script, enemy).init.push_back(Enemy::Level_0::init);
	GET_COMPONENT(Script, enemy).init.push_back(Enemy::Level_1::init);
	GET_COMPONENT(Script, enemy).update.push_back(Enemy::Level_0::update);
	GET_COMPONENT(Script, enemy).update.push_back(Enemy::Level_1::update);

	GET_COMPONENT(Script, lvlHandler).init.push_back(LevelHandler::Level_0::init);
	GET_COMPONENT(Script, lvlHandler).init.push_back(LevelHandler::Level_1::init);
	GET_COMPONENT(Script, lvlHandler).update.push_back(LevelHandler::Level_0::update);
	GET_COMPONENT(Script, lvlHandler).update.push_back(LevelHandler::Level_1::update);
/* ---------------------------------------------------------------------------------------------------- */

/* ------------------------------------ Init components of Entities ------------------------------------ */
	GET_COMPONENT(Transform, player).Static = false;
	GET_COMPONENT(Transform, enemy).Static = false;
	GET_COMPONENT(Transform, background).Static = false;

	for (uint32_t i = 0; i < 400; i++)
		GET_COMPONENT(Transform, wall[i]).Static = true;

	for (uint32_t i = 0; i < 10000; i++)
		GET_COMPONENT(Transform, level[i]).Static = true;

/* ----------------------------------------------------------------------------------------------------- */

/* ------------------------------------ Start Systems ------------------------------------ */
	windowSystem.Start(0);
	textureSystem.Start();
	renderingSystem.Start(cameraSystem.Get_View_Projection_Matrix());
	scriptingSystem.Start(levelSystem.GetCurrentLevel());
	collisionSystem.Start();
	gravitySystem.Start(6.0f, -6.0f);
	cameraSystem.Start();
	soundSystem.Start();
	transformSystem.Start();
	shadowSystem.Start(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), GET_COMPONENT(Transform, player).position, renderingSystem.Get_Vertex_Buffer(), renderingSystem);
/* --------------------------------------------------------------------------------------- */

	static double limitFPS = 1.0 / 60.0;

	double lastTime = glfwGetTime();
	double timer = lastTime;
	double deltaTime = 0, nowTime = 0;
	int frames = 0, updates = 0;

	while (!windowSystem.Window_Should_Close()) {
		// - Measure time
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / limitFPS;
		lastTime = nowTime;

		// - Only update at 60 frames / s
		while (deltaTime >= 1.0) {			
/* ------------------------------------ Run Systems ------------------------------------ */
			scriptingSystem.Run(levelSystem.GetCurrentLevel());
			textureSystem.Run(renderingSystem.Get_Vertex_Buffer());
			gravitySystem.Run();
			transformSystem.Run(renderingSystem.Get_Vertex_Buffer());
			collisionSystem.Run(renderingSystem.Get_Vertex_Buffer());
			shadowSystem.Run(GET_COMPONENT(Transform, player).position, renderingSystem.Get_Vertex_Buffer(), renderingSystem);
/* ------------------------------------------------------------------------------------- */
			updates++;
			deltaTime--;
		}

		// - Render at maximum possible frames
		renderingSystem.Render(cameraSystem.Get_View_Projection_Matrix());
		frames++;

		// - Reset after one second
		if (glfwGetTime() - timer > 1.0) {
			timer++;
			ENGINE_LOG("FPS: %d Updates: %d", frames, updates);
			updates = 0, frames = 0;
		}

		windowSystem.Swap_Buffers();
		windowSystem.Poll_For_Events();
	}

/* ------------------------------------ Clear Systems ------------------------------------ */
	renderingSystem.Clear();
	textureSystem.Clear();
	windowSystem.Terminate();
/* --------------------------------------------------------------------------------------- */
	return 0;
}

/**

struct Entity_player {
int ID;
int Transform;
int Renderable;
int CollisionBox;
}

std::vector<Component::Transform> transform;
.
.
.


int main() {
Entity_player ep;
ecs.enrollEntity(ep.ID);
ecs.addComponenet<Component::Transform>(ep.Transform);


}
----------------------------------------------------------------------

int main() {
	ecs = System();

	cams = ...
	texs = ...
	rends = ...
	cols = ...
	.
	.
	.
	wins = ...

	// enroll systems
	// enroll entities

	// add components to entities

	ecs.StartSystems();

	while() {

		ecs.RunSystems();

	}
}


*/