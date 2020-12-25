#include "Runtime/Core/Application/GlobalConfig.h"
#include "WindowsApplication.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/RHI/opengl/GraphicsMgrGL.h"
#include "Runtime/Core/Object/World.h"
#include "Foundation/Assert.h"
#include <windowsx.h>

using namespace scarlett;

static LRESULT CALLBACK TmpWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}

	return 0;
}

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
		case WM_LBUTTONDOWN: {
			pThis->mInputManager->OnKeyDown(KEY_CODE_LBUTTON);
			return 0;
		}
		case WM_LBUTTONUP: {
			pThis->mInputManager->OnKeyUp(KEY_CODE_LBUTTON);
			return 0;
		}

		case WM_RBUTTONDOWN: {
			pThis->mInputManager->OnKeyDown(KEY_CODE_RBUTTON);
			return 0;
		}

		case WM_RBUTTONUP: {
			pThis->mInputManager->OnKeyUp(KEY_CODE_RBUTTON);
			return 0;
		}

		case WM_KEYDOWN:
			pThis->mInputManager->OnKeyDown(wParam);
			return 0;
		case WM_KEYUP:
			pThis->mInputManager->OnKeyUp(wParam);
			return 0;
		case WM_MOUSEMOVE:
		{
			auto xPos = GET_X_LPARAM(lParam);
			auto yPos = GET_Y_LPARAM(lParam);
			pThis->mInputManager->OnMouseMove(xPos, yPos);
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

	debugWindow = new DebugWindow();
	debugWindow->Initialize();

	mVM = new ScriptEngineLua();
	mVM->Initialize();

	CreateMainWindow();

	mInputManager = new InputMgr();
	mInputManager->Initialize();

	mUseOpengl = false;
	if (mUseOpengl) {
		LoadWGL();
		CreateGLContext();
		mGraphicsManager = new GraphicsMgrGL();
		auto mgr = (GraphicsMgrGL*)mGraphicsManager;
		mgr->Initialize();
	}
	else {
		mGraphicsManager = new GraphicsMgrD11();
		auto mgr = (GraphicsMgrD11*)mGraphicsManager;
		mgr->InitializeWithWindow(mHWND);
	}

	mTimeMgr = new TimeMgr();
	mTimeMgr->Initialize();

	mFontMgr = new FontMgr();
	mFontMgr->Initialize();

	Director::GetInstance()->Initialize();

	mWorld = new World();
	mWorld->Initialize();

	return 0;
}

void WindowsApplication::Tick() noexcept{

	mTimeMgr->PreTick();
	Application::Tick();
	MSG msg = {};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	mWorld->Tick();
	Director::GetInstance()->Tick();
	Render();
	mTimeMgr->Tick();

	mTimeMgr->PostTick();
	Director::GetInstance()->PostTick();
}

void WindowsApplication::Render() noexcept{
	mWorld->Render();
	Director::GetInstance()->OnRender();

	mGraphicsManager->Present();

	if (mUseOpengl) {
		auto m_hDC = GetDC(mHWND);
		SwapBuffers(m_hDC);
	}
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

	int width = GlobalConfig::GetInstance()->GetScreenWidth();
	int height = GlobalConfig::GetInstance()->GetScreenHeight();
	RECT windowRect = { 0, 0, width, height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	mHWND = CreateWindowEx(
		NULL,
		windowClass.lpszClassName,
		GlobalConfig::GetInstance()->GetApplicationName().c_str(),
		WS_OVERLAPPEDWINDOW,
		100,
		100,
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

	mFontMgr->Finalize();
	mTimeMgr->Finalize();
	mInputManager->Finalize();
	mGraphicsManager->Finalize();
	mVM->Finalize();
	debugWindow->Finalize();
}

void WindowsApplication::LoadWGL() {
	int result = 0;
	DWORD Style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	WNDCLASSEX WndClassEx;
	memset(&WndClassEx, 0, sizeof(WNDCLASSEX));
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WndClassEx.lpfnWndProc = TmpWndProc;
	WndClassEx.hInstance = hInstance;
	WndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClassEx.lpszClassName = "InitWindow";
	RegisterClassEx(&WndClassEx);
	HWND TemphWnd = CreateWindowEx(WS_EX_APPWINDOW, WndClassEx.lpszClassName, "InitWindow", Style, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	HDC TemphDC = GetDC(TemphWnd);
	// Set a temporary default pixel format.
	int nPixelFormat = ChoosePixelFormat(TemphDC, &pfd);
	SCARLETT_ASSERT(nPixelFormat != 0);

	result = SetPixelFormat(TemphDC, nPixelFormat, &pfd);
	SCARLETT_ASSERT(result == 1);

	// Create a temporary rendering context.
	m_RenderContext = wglCreateContext(TemphDC);
	SCARLETT_ASSERT(result == 1);

	// Set the temporary rendering context as the current rendering context for this window.
	result = wglMakeCurrent(TemphDC, m_RenderContext);
	SCARLETT_ASSERT(result == 1);

	auto r = gladLoadWGL(TemphDC);
	SCARLETT_ASSERT(r != 0);
	
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_RenderContext);
	ReleaseDC(TemphWnd, TemphDC);
	DestroyWindow(TemphWnd);
}

void WindowsApplication::CreateGLContext() {
	int nPixelFormat;
	UINT numFormats;
	PIXELFORMATDESCRIPTOR pfd;
	int result;

	auto m_hDC = GetDC(mHWND);
	if (GLAD_WGL_ARB_pixel_format && GLAD_WGL_ARB_multisample && GLAD_WGL_ARB_create_context)
	{
		// enable MSAA
		const int attributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,     (int)24,
			WGL_RED_BITS_ARB,		(int)8,
			WGL_GREEN_BITS_ARB,		(int)8,
			WGL_BLUE_BITS_ARB,		(int)8,
			WGL_ALPHA_BITS_ARB,		(int)8,
			WGL_DEPTH_BITS_ARB,     (int)24,
			WGL_STENCIL_BITS_ARB,   (int)8,
			WGL_SAMPLE_BUFFERS_ARB, 1,
			WGL_SAMPLES_ARB,        4,  // 4x MSAA
			0
		};

		bool fail = wglChoosePixelFormatARB(m_hDC, attributes, nullptr, 1, &nPixelFormat, &numFormats) < 0 || numFormats == 0;
		SCARLETT_ASSERT(!fail);

		result = SetPixelFormat(m_hDC, nPixelFormat, &pfd);
		SCARLETT_ASSERT(result == 1);

		const int context_attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 1,
			WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		m_RenderContext = wglCreateContextAttribsARB(m_hDC, 0, context_attributes);
		SCARLETT_ASSERT(m_RenderContext);

		result = wglMakeCurrent(m_hDC, m_RenderContext);
		SCARLETT_ASSERT(result == 1);

	}
	else {
		// Set pixel format.
		int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
		SCARLETT_ASSERT(nPixelFormat != 0);

		result = SetPixelFormat(m_hDC, nPixelFormat, &pfd);
		SCARLETT_ASSERT(result == 1);

		// Create rendering context.
		m_RenderContext = wglCreateContext(m_hDC);
		SCARLETT_ASSERT(m_RenderContext);

		// Set the rendering context as the current rendering context for this window.
		result = wglMakeCurrent(m_hDC, m_RenderContext);
		SCARLETT_ASSERT(result == 1);
	}
}