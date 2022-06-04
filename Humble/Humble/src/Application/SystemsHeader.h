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

extern WindowSystem windowSystem;
extern TextureSystem textureSystem;
extern RenderingSystem renderingSystem;
extern CameraSystem cameraSystem;
extern ScriptingSystem scriptingSystem;
extern CollisionSystem collisionSystem;
extern GravitySystem gravitySystem;
extern TransformSystem transformSystem;
extern ShadowCastSystem shadowSystem;