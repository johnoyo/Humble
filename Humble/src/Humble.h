#pragma once

// Get rid of the console when the application is running on release mode
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "Core/GlobalArrays.h"
#include "Core/Utilities.h"
#include "Core/GlobalSystems.h"

#include "Core/IScript.h"
#include "Core/Managers/InputManager.h"
#include "Core/Managers/SoundManager.h"
#include "Core/Utilities/Random.h"

#include "Core/ISystem.h"

#include "Core/Application.h"
