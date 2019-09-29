#include "Dispatcher.h"

using namespace scarlett;

UIEventDispatch::UIEventDispatch() {
}

UIEventDispatch::~UIEventDispatch() {
}

void UIEventDispatch::addEventListener(UIEventListener* listener) {
	auto type = listener->getType();
	_listeners[type].push_back(listener);
	listener->Retain();
}

void UIEventDispatch::deleteEventListener(UIEventListener* listener) {
	auto type = listener->getType();
	auto listeners = _listeners[type];

	auto iter = listeners.begin();
	for (; iter != listeners.end(); ++iter) {
		if (*iter == listener) {
			listeners.erase(iter);
		}
	}
}

void UIEventDispatch::dispatchEvent(UIEvent* event) {
	auto type = event->_type;
	auto ltype = UIEventListener::Type::UNKNOWN;
	if (type == UIEvent::Type::MOUSE) {
		dispatchMouseEvent(event);
	}
	else if (type == UIEvent::Type::KEYBOARD) {
		dispatchKeyboardEvent(event);
	}
}

void UIEventDispatch::dispatchMouseEvent(UIEvent* event) {
	auto ltype = UIEventListener::Type::MOUSE;
	auto listeners = _listeners[ltype];

	auto iter = listeners.begin();
	for (; iter != listeners.end(); ++iter) {

	}
}

void UIEventDispatch::dispatchKeyboardEvent(UIEvent* event) {
	auto ltype = UIEventListener::Type::KEYBOARD;
	auto listeners = _listeners[ltype];

	auto iter = listeners.begin();
	for (; iter != listeners.end(); ++iter) {
		auto keyboardEvent = static_cast<UIEventKeyboard*>(event);
		auto keyboardListener = static_cast<UIEventListenerKeyboard*>(*iter);
		keyboardListener->_onEvent(event);
	}
}
