#pragma once 
#include <functional>
#include "Ref.h"
#include "Event.h"
#include "Node.h"

namespace scarlett {

	class UIEventListener : public Ref {

	public:
		enum class Type
		{
			UNKNOWN,
			TOUCH_ONE_BY_ONE,
			TOUCH_ALL_AT_ONCE,
			KEYBOARD,
			MOUSE,
			ACCELERATION,
			FOCUS,
			GAME_CONTROLLER,
			CUSTOM
		};
		typedef std::string ListenerID;

		UIEventListener();
		virtual ~UIEventListener();
		bool init(Type t, const ListenerID& listenerID, const std::function<void(UIEvent*)>& callback);
		Type getType() const { return _type; }
		void setTarget(UINode* target) { _target = target; }
		std::function<void(UIEvent*)> _onEvent;
	protected:
		Type _type;
		ListenerID _listenerID;
		UINode* _target;
	};


	class UIEventListenerKeyboard : public UIEventListener {
	public:
		UIEventListenerKeyboard();
		static UIEventListenerKeyboard* create();
		std::function<void(unsigned char, UIEvent*)> onKeyPressed;
		std::function<void(unsigned char, UIEvent*)> onKeyReleased;

	};

}