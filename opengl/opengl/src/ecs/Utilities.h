#pragma once

#include "Header.h"
#include "../ProfilerScope.h"
#include <string>

#define ENROLL_ENTITY(entity) ecs.EnrollEntity(entity, entities)

#define ADD_COMPONENT(component, entity) ecs.AddComponent<Component::component>(entity.component, entities.at(entity.ID).component, component)
#define GET_COMPONENT(component, entity) ecs.GetComponent<Component::component>(entity.component, component)
#define TRY_FIND_COMPONENT(component, entity) (entity.component != -1)

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef _DEBUG
#define ENGINE_LOG(f_, ...) printf(("ENGINE_LOG[%s]: " f_ " (%s::%d)\n"), __TIME__, ##__VA_ARGS__, __FILENAME__, __LINE__)
#define ENGINE_PROFILE(...) ProfilerScope profile = ProfilerScope(__VA_ARGS__);
#else
#define ENGINE_LOG(f_, ...)
#define ENGINE_PROFILE()
#endif // _DEBUG
