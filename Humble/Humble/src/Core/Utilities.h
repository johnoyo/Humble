#pragma once

#include "Header.h"
#include "ProfilerScope.h"
#include <string>

#define ENROLL_ENTITY(entity) Globals::ecs.EnrollEntity(entity, Globals::entities)

#define ADD_COMPONENT(component, entity) Globals::ecs.AddComponent<HBL::Component::component>(entity, #component, Globals::entities, Globals::component)
#define GET_COMPONENT(component, entity) Globals::ecs.GetComponent<HBL::Component::component>(entity, #component, Globals::component)

#define ADD_MY_COMPONENT(component, entity) Globals::ecs.AddComponent<HBL::Component::component>(entity, #component, Globals::entities, component)
#define GET_MY_COMPONENT(component, entity) Globals::ecs.GetComponent<HBL::Component::component>(entity, #component, component)

#define TRY_FIND_COMPONENT(component, entity) (entity.components.find(#component) != entity.components.end())

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef _DEBUG
#define ENGINE_LOG(f_, ...) printf(("ENGINE_LOG[%s]: " f_ " (%s::%d)\n"), __TIME__, ##__VA_ARGS__, __FILENAME__, __LINE__)
#define ENGINE_PROFILE(...) HBL::ProfilerScope profile = HBL::ProfilerScope(__VA_ARGS__);
#define FUNCTION_PROFILE() ENGINE_PROFILE(__FUNCTION__)
#else
#define ENGINE_LOG(f_, ...)
#define ENGINE_PROFILE(...)
#define FUNCTION_PROFILE(...)
#endif // _DEBUG
