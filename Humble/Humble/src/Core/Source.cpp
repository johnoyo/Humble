#include "Header.h"

HBL::System Globals::ecs;

std::vector<HBL::IEntity> Globals::entities;

bool Globals::Scene_Change = false;
uint32_t Globals::Current_Level = 0;

std::vector<HBL::Component::Transform> Globals::Transform;
std::vector<HBL::Component::CollisionBox> Globals::CollisionBox;
std::vector<HBL::Component::Material> Globals::Material;
std::vector<HBL::Component::Animation> Globals::Animation;
std::vector<HBL::Component::Script> Globals::Script;
std::vector<HBL::Component::Gravity> Globals::Gravity;
std::vector<HBL::Component::Shadow> Globals::Shadow;
std::vector<HBL::Component::Text> Globals::Text;

