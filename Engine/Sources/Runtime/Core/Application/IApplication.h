#pragma once
#include "Foundation/Assert.h"

namespace scarlett {

#define CHECK_APPLICATION_INIT(result)	if(result!=0){ exit(result);}
	
	class IApplication {
	public:
		virtual int Initialize() noexcept = 0;
		virtual void Run() noexcept = 0;
		virtual void Tick() noexcept = 0;
		virtual void Render() noexcept = 0;
		virtual void Finalize() noexcept = 0;
		virtual void Quit() noexcept = 0;
		virtual bool IsQuit() noexcept = 0;
	};
}