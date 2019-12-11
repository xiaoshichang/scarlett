#include "Application.h"

using namespace scarlett;
int Application::Initialize() noexcept {
	mQuit = false;
	mGraphicsManager = nullptr;
	mDirector = nullptr;
	mInputManager = nullptr;
	mFontMgr = nullptr;
	mWorld = nullptr;
	return 0;
}


void Application::Tick() noexcept{

}

void Application::Render() noexcept{

}

void Application::Run() noexcept{
	while (!IsQuit())
	{
		Tick();
	}
}

void Application::Finalize() noexcept {
}

bool Application::IsQuit() noexcept {
	return mQuit;
}

void Application::Quit() noexcept {
	mQuit = true;
}