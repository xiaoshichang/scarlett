#include "InputMgr.h"
#include <iostream>
#include "Runtime/Utils/Logging.h"

#include "Runtime/Core/UI/Director.h"
#include "Runtime/Core/UI/Event.h"
#include "Runtime/Core/Application/Application.h"

using namespace std;


int scarlett::InputMgr::Initialize() noexcept
{
	mDispatcher = new UIEventDispatch();

	SCARLETT_LOG(info) << "InputMgr Initialize ok.";
	return 0;
}

void scarlett::InputMgr::Finalize() noexcept
{
}

void scarlett::InputMgr::OnKeyDown(unsigned char keyCode)
{
	cout << "key down: " << int(keyCode) << endl;

	UIEventKeyboard event(keyCode, true);
	Director::GetInstance()->GetDispatcher()->dispatchEvent(&event);
	mDispatcher->dispatchEvent(&event);
	
}

void scarlett::InputMgr::OnKeyUp(unsigned char keyCode)
{
	cout << "key up: " << int(keyCode) << endl;

	UIEventKeyboard event(keyCode, false);
	Director::GetInstance()->GetDispatcher()->dispatchEvent(&event);
	mDispatcher->dispatchEvent(&event);
}

void scarlett::InputMgr::OnMouseMove(int x, int y)
{
}