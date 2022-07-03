#include "Header.h"

HBL::System Globals::ecs;

std::vector<HBL::IEntity> Globals::entities;

bool Globals::Scene_Change = false;

std::vector<HBL::Component::Transform> Globals::Transform;
std::vector<HBL::Component::CollisionBox> Globals::CollisionBox;
std::vector<HBL::Component::Material> Globals::Material;
std::vector<HBL::Component::Health> Globals::Health;
std::vector<HBL::Component::Script> Globals::Script;
std::vector<HBL::Component::Gravity> Globals::Gravity;
std::vector<HBL::Component::Shadow> Globals::Shadow;
