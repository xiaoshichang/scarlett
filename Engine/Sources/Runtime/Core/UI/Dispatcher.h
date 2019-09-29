#pragma once
#include <unordered_map>
#include <vector>
#include "Ref.h"
#include "Event.h"
#include "Listener.h"


namespace scarlett {
	class UIEventDispatch : public Ref {

	public:
		void addEventListener(UIEventListener* listener);
		void deleteEventListener(UIEventListener* listener);
		void dispatchEvent(UIEvent* event);
		void dispatchMouseEvent(UIEvent* event);
		void dispatchKeyboardEvent(UIEvent* event);

		UIEventDispatch();
		virtual ~UIEventDispatch();
	public:

		std::unordered_map<UIEventListener::Type, std::vector<UIEventListener*>> _listeners;
	};

}