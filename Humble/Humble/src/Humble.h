#pragma once

// Get rid of the console when the application is running on release mode
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

//// Force discrete GPU on Laptops
//#ifdef HBL_PLATFORM_WINDOWS
//#include <windows.h>
//extern "C"
//{
//	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//}
//#endif //def HBL_PLATFORM_WINDOWS

#include "Core/Header.h"
#include "Core/Utilities.h"
#include "Core/SystemsHeader.h"

#include "Core/ISystem.h"

#include "Core/Application.h"

