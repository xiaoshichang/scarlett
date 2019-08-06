#pragma once

#define SCARLETT_PLATFORM_UNKNOWN				"unknown"
#define SCARLETT_PLATFORM_WINDOWS				"windows"
#define SCARLETT_PLATFORM_MACOS					"macos"
#define SCARLETT_PLATFORM_IOS					"ios"
#define SCARLETT_PLATFORM_ANDROID				"android"

#if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)
	#define SCARLETT_PLATFORM	SCARLETT_PLATFORM_WINDOWS
	#define SCARLETT_WINDOWS
#elif  defined(__APPLE__)
	#define SCARLETT_PLATFORM	SCARLETT_PLATFORM_MACOS
	#define SCARLETT_MACOS
#else
	#define SCARLETT_PLATFORM	SCARLETT_PLATFORM_UNKNOWN
	#define SCARLETT_UNKNOWN
#endif

#if defined(SCARLETT_WINDOWS)
	#include "Foundation/Platform/Windows.h"
#elif defined(SCARLETT_MACOS)
	#include "Foundation/Platform/Macos.h"
#else
#endif
