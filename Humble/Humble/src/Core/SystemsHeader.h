#pragma once
#include "..\Core\Systems\CameraSystem.h"
#include "..\Core\Systems\RenderingSystem.h"
#include "..\Core\Systems\WindowSystem.h"
#include "..\Core\Systems\TextureSystem.h"
#include "..\Core\Systems\ScriptingSystem.h"
#include "..\Core\Systems\CollisionSystem.h"
#include "..\Core\Systems\GravitySystem.h"
#include "..\Core\Managers\SoundManager.h"
#include "..\Core\Systems\TransformSystem.h"
#include "..\Core\Systems\ShadowCastSystem.h"

struct HBL_API GlobalSystems 
{
	static HBL::WindowSystem windowSystem;
	static HBL::TextureSystem textureSystem;
	static HBL::RenderingSystem renderingSystem;
	static HBL::CameraSystem cameraSystem;
	static HBL::ScriptingSystem scriptingSystem;
	static HBL::CollisionSystem collisionSystem;
	static HBL::GravitySystem gravitySystem;
	static HBL::TransformSystem transformSystem;
	static HBL::ShadowCastSystem shadowSystem;
};
