#pragma once

#include "..\Core\Systems\TextSystem.h"
#include "..\Core\Systems\CameraSystem.h"
#include "..\Core\Systems\WindowSystem.h"
#include "..\Core\Systems\GravitySystem.h"
#include "..\Core\Managers\SoundManager.h"
#include "..\Core\Systems\TransformSystem.h"
#include "..\Core\Systems\AnimationSystem.h"
#include "..\Core\Systems\ScriptingSystem.h"
#include "..\Core\Systems\CollisionSystem.h"
#include "..\Core\Systems\ShadowCastSystem.h"
#include "..\Core\Systems\SpriteRendererSystem.h"
#include "..\Core\Systems\ClickableSystem.h"

struct HBL_API Systems
{
	static HBL::WindowSystem Window;
	static HBL::CameraSystem Camera;
	static HBL::SpriteRendererSystem SpriteRenderer;
	static HBL::AnimationSystem Animation;
	static HBL::ScriptingSystem Scripting;
	static HBL::CollisionSystem Collision;
	static HBL::GravitySystem Gravity;
	static HBL::TransformSystem Transform;
	static HBL::ShadowCastSystem Shadow;
	static HBL::TextSystem Text;
	static HBL::ClickableSystem Clickable;
};
