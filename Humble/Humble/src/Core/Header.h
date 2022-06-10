#pragma once

#include "System.h"
#include "Entities.h"
#include "Components.h"
#include <vector>
#include <string>

extern HBL::System ecs;

extern std::vector<HBL::Entity::BaseEntity> entities;

/* ------------------------- Component Vector Declaration -------------------------- */
extern std::vector<HBL::Component::Transform> Transform;
extern std::vector<HBL::Component::CollisionBox> CollisionBox;
extern std::vector<HBL::Component::Material> Material;
extern std::vector<HBL::Component::Health> Health;
extern std::vector<HBL::Component::Script> Script;
extern std::vector<HBL::Component::Gravity> Gravity;
extern std::vector<HBL::Component::Shadow> Shadow;
/* --------------------------------------------------------------------------------- */

static bool Scene_Change = false;
