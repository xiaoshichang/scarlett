#pragma once

#include "Application.h"
#include "Runtime/Utils/Singleton.h"
#include <Windows.h>
#include "glad/glad_wgl.h"
#include "Runtime/Core/Debug/DebugWindow.h"


namespace scarlett {
	class WindowsApplication : public Application {
	public:
		WindowsApplication() {}

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;
		virtual void Render() noexcept;

		HWND GetWindowsHandler() noexcept;
	
	protected:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		void CreateMainWindow();

		void LoadWGL();
		void CreateGLContext();

	private:
		HWND mHWND;
		HGLRC m_RenderContext;
		DebugWindow* debugWindow;

		bool mUseOpengl;
	};

}