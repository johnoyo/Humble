#pragma once

// Get rid of the console when the application is running on release mode
#ifndef DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "Core/Systems.h"
#include "Core/Utilities.h"

#include "Core/ECS/ISystem.h"
#include "Core/ECS/Registry.h"

#include "Core/IScript.h"
#include "Core/Utilities/Random.h"
#include "Core/Managers/InputManager.h"
#include "Core/Managers/SoundManager.h"
#include "Core/Managers/SceneManager.h"

#include "Core/Application.h"
