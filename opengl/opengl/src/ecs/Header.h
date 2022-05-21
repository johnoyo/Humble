#pragma once

#include "System.h"
#include "Entities.h"
#include "Components.h"
#include <vector>
#include <string>

extern System ecs;

extern std::vector<Entity::BaseEntity> entities;

/* ------------------------------- Entities Declaration ---------------------------- */
extern Entity::BaseEntity background;
extern Entity::BaseEntity player;
extern Entity::BaseEntity enemy;
extern Entity::BaseEntity wall[400];
extern Entity::BaseEntity level[10000];
extern Entity::BaseEntity lvlHandler;
extern Entity::BaseEntity camera;
extern Entity::BaseEntity sps;
/* --------------------------------------------------------------------------------- */

/* ------------------------- Component Vector Declaration -------------------------- */
extern std::vector<Component::Transform> Transform;
extern std::vector<Component::CollisionBox> CollisionBox;
extern std::vector<Component::Material> Material;
extern std::vector<Component::Health> Health;
extern std::vector<Component::Script> Script;
extern std::vector<Component::Gravity> Gravity;
extern std::vector<Component::Shadow> Shadow;
/* --------------------------------------------------------------------------------- */
