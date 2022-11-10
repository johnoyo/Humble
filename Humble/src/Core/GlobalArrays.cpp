#include "GlobalArrays.h"

HBL::Registry Globals::s_Registry;

bool Globals::Scene_Change = false;
uint32_t Globals::Current_Level = 0;
HBL::IEntity Globals::Camera;
