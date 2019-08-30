#pragma once

#include <string>
using namespace std;

namespace scarlett {

	/*
	a render instance, including all things needed to render a mesh.
	*/
	class RenderObject {
	public:
		virtual void Render();

	public:
		string	GetName() { return mName; }
		void	SetName(string name) { mName = name; }

		bool IsVisible() { return mVisible; }
		void SetVisible(bool v) { mVisible = v; }

	private:
		string mName;
		bool mVisible;

	};

}