#include "InputMgr.h"
#include <iostream>
#include "Runtime/Utils/Logging.h"
using namespace std;


int scarlett::InputMgr::Initialize() noexcept
{
	SCARLETT_LOG(info) << "InputMgr Initialize ok.";
	return 0;
}

void scarlett::InputMgr::Finalize() noexcept
{
}

void scarlett::InputMgr::OnKeyDown(unsigned char keyCode)
{
	cout << "key down: " << int(keyCode) << endl;
}

void scarlett::InputMgr::OnKeyUp(unsigned char keyCode)
{
	cout << "key up: " << int(keyCode) << endl;
}

void scarlett::InputMgr::OnMouseMove(int x, int y)
{
}