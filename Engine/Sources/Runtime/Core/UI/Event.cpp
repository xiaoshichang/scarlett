#include "Event.h"

using namespace scarlett;

UIEventMouse::UIEventMouse(MouseEventType mouseEventCode)
	: UIEvent(UIEvent::Type::MOUSE),
		_mouseEventType(mouseEventCode)
{
}

UIEventKeyboard::UIEventKeyboard(unsigned char code, bool isPressed)
	: UIEvent(UIEvent::Type::KEYBOARD),
		_keyCode(code),
		_isPressed(isPressed)
{
}