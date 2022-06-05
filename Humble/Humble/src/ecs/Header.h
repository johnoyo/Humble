#pragma once

#include "System.h"
#include "Entities.h"
#include "Components.h"
#include <vector>
#include <string>

extern HBL::System ecs;

extern std::vector<HBL::Entity::BaseEntity> entities;

/* ------------------------------- Entities Declaration ---------------------------- */
extern HBL::Entity::BaseEntity background;
extern HBL::Entity::BaseEntity player;
extern HBL::Entity::BaseEntity enemy;
extern HBL::Entity::BaseEntity wall[400];
extern HBL::Entity::BaseEntity level[10000];
extern HBL::Entity::BaseEntity lvlHandler;
extern HBL::Entity::BaseEntity camera;
extern HBL::Entity::BaseEntity sps;
/* --------------------------------------------------------------------------------- */

/* ------------------------- Component Vector Declaration -------------------------- */
extern std::vector<HBL::Component::Transform> Transform;
extern std::vector<HBL::Component::CollisionBox> CollisionBox;
extern std::vector<HBL::Component::Material> Material;
extern std::vector<HBL::Component::Health> Health;
extern std::vector<HBL::Component::Script> Script;
extern std::vector<HBL::Component::Gravity> Gravity;
extern std::vector<HBL::Component::Shadow> Shadow;
/* --------------------------------------------------------------------------------- */
