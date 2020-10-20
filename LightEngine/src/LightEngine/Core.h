#pragma once
#ifdef LE_PLATFORM_WINDOWS
	#ifdef LE_BUILD_DLL
		#define LIGHTENGINE_API __declspec(dllexport)
	#else
		#define LIGHTENGINE_API __declspec(dllimport) 
	#endif // LE_BUILD_DLL
#else
	#error LightEngine only supports Windows!
#endif // LE_PLATFORM_WINDOWS
