#pragma once

#include "IApplication.h"
#include "Runtime/Core/Memory/MemoryManager.h"
#include "Runtime/RHI/GraphicsMgr.h"
#include "Runtime/Core/Time/TimeMgr.h"
#include "Runtime/Core/Input/InputMgr.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/UI/Director.h"

namespace scarlett {
	class Application : public IApplication {

	public:
		virtual int		Initialize() noexcept;
		virtual void	Tick() noexcept;
		virtual void	Render() noexcept;
		virtual void	Run() noexcept;
		virtual void	Finalize() noexcept;
		virtual void	Quit() noexcept;
		virtual bool	IsQuit() noexcept;

	public:
		GraphicsManager*	mGraphicsManager;
		TimeMgr*			mTimeMgr;
		InputMgr*			mInputManager;
		Director*			mDirector;
		World*				mWorld;

	private:
		bool mQuit;

	};

	extern Application* GApp;
}