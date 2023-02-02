#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <algorithm>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Utilities/ProfilerScope.h"

#define X_AXIS 1
#define Y_AXIS 2
#define PI 3.14159265f

#define ASSERT(x) if (!(x)) __debugbreak();

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
