#pragma once

// Force discrete GPU on Laptops
#ifdef HBL_PLATFORM_WINDOWS
//#include <windows.h>
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif //def HBL_PLATFORM_WINDOWS