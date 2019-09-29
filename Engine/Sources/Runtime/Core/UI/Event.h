#pragma once

#include "Ref.h"

namespace scarlett {

	class UIEvent : public Ref {
	public:
		enum class Type
		{
			TOUCH,
			KEYBOARD,
			ACCELERATION,
			MOUSE,
			FOCUS,
			GAME_CONTROLLER,
			CUSTOM
		};

	public:
		UIEvent(Type type) : _type(type){}
		virtual ~UIEvent() {};

	public:
		Type _type;
	};

	class UIEventMouse : public UIEvent {
	public:
		enum class MouseEventType
		{
			MOUSE_NONE,
			MOUSE_DOWN,
			MOUSE_UP,
			MOUSE_MOVE,
			MOUSE_SCROLL,
		};

		enum class MouseButton
		{
			BUTTON_UNSET = -1,
			BUTTON_LEFT = 0,
			BUTTON_RIGHT = 1,
			BUTTON_MIDDLE = 2,
			BUTTON_4 = 3,
			BUTTON_5 = 4,
			BUTTON_6 = 5,
			BUTTON_7 = 6,
			BUTTON_8 = 7
		};

	public:
		UIEventMouse(MouseEventType mouseEventCode);


	public:
		MouseEventType _mouseEventType;
		MouseButton _mouseButton;
		float _x;
		float _y;
	};

	class UIEventKeyboard : public UIEvent {
	public:

		UIEventKeyboard(unsigned char code, bool isPressed);

	public:
		unsigned char _keyCode;
		bool _isPressed;


	};
}