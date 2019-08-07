#pragma once
#include "Foundation/Platform.h"
#include "Foundation/BuildType.h"

#if defined(SCARLETT_WINDOWS)

#include <crtdbg.h>
#define SCARLETT_ASSERT_IMPL(EXPR)		_ASSERTE(EXPR)

#elif defined(SCARLETT_MACOS)
#include <assert.h>
#define SCARLETT_ASSERT_IMPL(EXPR)      assert(EXPR)
#else

#endif


#if defined(SCARLETT_DEBUG)
#define SCARLETT_ASSERT(EXPR)		SCARLETT_ASSERT_IMPL(EXPR)
#elif defined(SCARLETT_RELEASE)
#define SCARLETT_ASSERT(EXPR)		((void)0)
#else
#endif