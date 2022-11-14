#pragma once

#include "..\Core\Systems\CameraSystem.h"
#include "..\Core\Systems\SpriteRendererSystem.h"
#include "..\Core\Systems\WindowSystem.h"
#include "..\Core\Systems\AnimationSystem.h"
#include "..\Core\Systems\ScriptingSystem.h"
#include "..\Core\Systems\CollisionSystem.h"
#include "..\Core\Systems\GravitySystem.h"
#include "..\Core\Managers\SoundManager.h"
#include "..\Core\Systems\TransformSystem.h"
#include "..\Core\Systems\ShadowCastSystem.h"
#include "..\Core\Systems\TextSystem.h"

struct HBL_API GlobalSystems
{
	static HBL::WindowSystem windowSystem;
	static HBL::CameraSystem cameraSystem;
	static HBL::SpriteRendererSystem spriteRendererSystem;
	static HBL::AnimationSystem animationSystem;
	static HBL::ScriptingSystem scriptingSystem;
	static HBL::CollisionSystem collisionSystem;
	static HBL::GravitySystem gravitySystem;
	static HBL::TransformSystem transformSystem;
	static HBL::ShadowCastSystem shadowSystem;
	static HBL::TextSystem textSystem;
};
