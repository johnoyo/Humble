#pragma once

// Get rid of the console when the application is running on release mode
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "Core/Header.h"
#include "Core/Utilities.h"
#include "Core/SystemsHeader.h"

#include "Core/ScriptFunction.h"
#include "Core/Managers/InputManager.h"
#include "Core/Managers/SoundManager.h"

#include "Core/ISystem.h"

#include "Core/Application.h"
