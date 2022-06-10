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

extern HBL::Entity::BaseEntity background;
extern HBL::Entity::BaseEntity player;
extern HBL::Entity::BaseEntity enemy;
extern HBL::Entity::BaseEntity wall[400];
extern HBL::Entity::BaseEntity level[10000];
extern HBL::Entity::BaseEntity camera;
extern HBL::Entity::BaseEntity lvlHandler;
extern HBL::Entity::BaseEntity sps;