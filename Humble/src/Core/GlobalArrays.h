#pragma once

#include "ECS/IEntity.h"
#include "ECS/Registry.h"
#include "ECS/Components.h"

#include <vector>
#include <string>

struct HBL_API Globals 
{
	static bool Scene_Change;
	static uint32_t Current_Level;
	static HBL::IEntity Camera;
};
