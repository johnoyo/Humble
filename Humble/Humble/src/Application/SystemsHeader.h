#pragma once
#include "..\ecs\Systems\CameraSystem.h"
#include "..\ecs\Systems\RenderingSystem.h"
#include "..\ecs\Systems\WindowSystem.h"
#include "..\ecs\Systems\TextureSystem.h"
#include "..\ecs\Systems\ScriptingSystem.h"
#include "..\ecs\Systems\CollisionSystem.h"
#include "..\ecs\Systems\GravitySystem.h"
#include "..\ecs\Managers\SoundManager.h"
#include "..\ecs\Systems\TransformSystem.h"
#include "..\ecs\Systems\ShadowCastSystem.h"

extern HBL::WindowSystem windowSystem;
extern HBL::TextureSystem textureSystem;
extern HBL::RenderingSystem renderingSystem;
extern HBL::CameraSystem cameraSystem;
extern HBL::ScriptingSystem scriptingSystem;
extern HBL::CollisionSystem collisionSystem;
extern HBL::GravitySystem gravitySystem;
extern HBL::TransformSystem transformSystem;
extern HBL::ShadowCastSystem shadowSystem;