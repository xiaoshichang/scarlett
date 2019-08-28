#include "WindowsApplication.h"
#include "Runtime/RHI/GraphicsMgrD11.h"
using namespace scarlett;

LRESULT scarlett::WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowsApplication* pThis;
	pThis = reinterpret_cast<WindowsApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	switch (message) {
		case WM_CREATE: {
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			pThis->Quit();
			return 0;
		}
	}

	// Handle any messages the switch statement didn't.
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WindowsApplication::Initialize() noexcept{
	CHECK_APPLICATION_INIT(Application::Initialize());
	CreateMainWindow();

	mMemoryMgr = new MemoryManager();
	mMemoryMgr->Initialize();

	mGraphicsManager = new GraphicsMgrD11();
	auto mgr = (GraphicsMgrD11*)mGraphicsManager;
	mgr->InitializeWithWindow(mHWND);

	mWorld = new World();
	mWorld->Initialize();
	mWorld->LoadScene("scene");

	return 0;
}

void WindowsApplication::Tick() noexcept{
	Application::Tick();
	MSG msg = {};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	mWorld->Tick();
	Render();
}

void WindowsApplication::Render() noexcept{
	mWorld->Render();
}

HWND scarlett::WindowsApplication::GetWindowsHandler() noexcept
{
	return mHWND;
}

void scarlett::WindowsApplication::CreateMainWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = "scarlett";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, 1024, 768 };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	mHWND = CreateWindow(
		windowClass.lpszClassName,
		"scarlett",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,        // We have no parent window.
		nullptr,        // We aren't using menus.
		hInstance,
		this);

	// Initialize the sample. OnInit is defined in each child-implementation of DXSample.
	ShowWindow(mHWND, 5);
}

void WindowsApplication::Finalize() noexcept{
	mWorld->Finalize();

	mGraphicsManager->Finalize();
	mMemoryMgr->Finalize();
	
}