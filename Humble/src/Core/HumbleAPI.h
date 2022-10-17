#pragma once

#ifdef HBL_PLATFORM_WINDOWS
	#ifdef HBL_BUILD_DLL
		#define HBL_API __declspec(dllexport) 
	#else
		#define HBL_API __declspec(dllimport) 
	#endif // HBL_BUILD_DLL
#else
	#error Humble only supports Windows!
#endif // HBL_PLATFORM_WINDOWS

