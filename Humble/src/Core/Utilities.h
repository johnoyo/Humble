#pragma once

#include "Utilities/ProfilerScope.h"
#include <string>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef DEBUG
#define ENGINE_LOG(f_, ...) printf(("ENGINE_LOG[%s]: " f_ " (%s::%d)\n"), __TIME__, ##__VA_ARGS__, __FILENAME__, __LINE__)
#define ENGINE_PROFILE(...) HBL::ProfilerScope profile = HBL::ProfilerScope(__VA_ARGS__);
#define FUNCTION_PROFILE() ENGINE_PROFILE(__FUNCTION__)
#else
#define ENGINE_LOG(f_, ...)
#define ENGINE_PROFILE(...)
#define FUNCTION_PROFILE()
#endif // _DEBUG
