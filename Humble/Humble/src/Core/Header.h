#pragma once

#include "System.h"
#include "IEntity.h"
#include "Components.h"
#include <vector>
#include <string>

struct HBL_API Globals 
{
	static HBL::System ecs;

	static std::vector<HBL::IEntity> entities;

	static bool Scene_Change;

	// Component Vector Declaration
	static std::vector<HBL::Component::Transform> Transform;
	static std::vector<HBL::Component::CollisionBox> CollisionBox;
	static std::vector<HBL::Component::Material> Material;
	static std::vector<HBL::Component::Health> Health;
	static std::vector<HBL::Component::Script> Script;
	static std::vector<HBL::Component::Gravity> Gravity;
	static std::vector<HBL::Component::Shadow> Shadow;
};
