#include "DebugWindow.h"
#include <iostream>
#include "Runtime/Core/Application/Application.h"


int scarlett::DebugWindow::Initialize() noexcept
{
#if defined(SCARLETT_WINDOWS)
	InitConsoleWindow();
#endif
	return 0;
}

void scarlett::DebugWindow::Finalize() noexcept
{

}


#if defined(SCARLETT_WINDOWS)

void scarlett::DebugWindow::InitConsoleWindow()
{
	AllocConsole();
	consoleHandle = GetConsoleWindow();

	SetConsoleTitle("scarlett debug window");


	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stderr);
	freopen("CONOUT$", "w", stdout);

	worker = new std::thread(&scarlett::DebugWindow::Run, this);
}

void scarlett::DebugWindow::Run()
{
	while (true)
	{
		// 暂时先这样吧，勉强能用
		std::cout << ">>>";
		std::getline(std::cin, cmdBuffer);
		GApp->mVM->Eval(cmdBuffer);
	}
}


void scarlett::DebugWindow::ReleaseConsoleWindow()
{

}

#endif