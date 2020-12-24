#pragma once
#include "Foundation/Platform.h"
#include "Runtime/Interface/IModule.h"

#if defined(SCARLETT_WINDOWS)
#include <Windows.h>
#include <thread> 
#include <string>
#endif


namespace scarlett
{
	class DebugWindow : public IModule
	{
	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

#if defined(SCARLETT_WINDOWS)
	private:
		void InitConsoleWindow();
		void ReleaseConsoleWindow();
		void Run();

#endif
	private:

#if defined(SCARLETT_WINDOWS)
		HWND consoleHandle;
		std::thread* worker;
		std::string cmdBuffer;
#endif

	};

}