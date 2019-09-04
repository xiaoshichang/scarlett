#pragma once

#include <string>

namespace scarlett {

	/*
	a render instance, including all things needed to render.
	*/
	class RenderObject {
	public:
		virtual void Render();

	public:
		std::string	GetName() { return mName; }
		void	SetName(std::string name) { mName = name; }

		bool IsVisible() { return mVisible; }
		void SetVisible(bool v) { mVisible = v; }

	private:
		std::string mName;
		bool mVisible;

	};

}