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

extern HBL::WindowSystem windowSystem;
extern HBL::TextureSystem textureSystem;
extern HBL::RenderingSystem renderingSystem;
extern HBL::CameraSystem cameraSystem;
extern HBL::ScriptingSystem scriptingSystem;
extern HBL::CollisionSystem collisionSystem;
extern HBL::GravitySystem gravitySystem;
extern HBL::TransformSystem transformSystem;
extern HBL::ShadowCastSystem shadowSystem;
