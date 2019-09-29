#include "Listener.h"

scarlett::UIEventListener::UIEventListener()
	:_onEvent(nullptr),
	_type(Type::UNKNOWN),
	_listenerID(""),
	_target(nullptr)
{
}

scarlett::UIEventListener::~UIEventListener() {

}

bool scarlett::UIEventListener::init(Type t, const ListenerID& listenerID, const std::function<void(UIEvent*)>& callback) {
	_onEvent = callback;
	_type = t;
	_listenerID = listenerID;
	return true;
}


scarlett::UIEventListenerKeyboard::UIEventListenerKeyboard()
	: onKeyPressed(nullptr)
	, onKeyReleased(nullptr)
{
	auto listener = [this](UIEvent* event) {
		auto keyboardEvent = static_cast<UIEventKeyboard*>(event);
		if (keyboardEvent->_isPressed)
		{
			if (onKeyPressed != nullptr)
				onKeyPressed(keyboardEvent->_keyCode, event);
		}
		else
		{
			if (onKeyReleased != nullptr)
				onKeyReleased(keyboardEvent->_keyCode, event);
		}
	};
	if (UIEventListener::init(Type::KEYBOARD, "", listener))
	{
		return;
	}
}

scarlett::UIEventListenerKeyboard* scarlett::UIEventListenerKeyboard::create() {
	auto ret = new scarlett::UIEventListenerKeyboard();
	return ret;
}