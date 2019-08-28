#pragma once

namespace scarlett {

	/*
	a render instance, including all things needed to render a mesh.
	*/
	class RenderObject {
	public:
		virtual void Render();

	public:
		bool IsVisible() { return mVisible; }
		void SetVisible(bool v) { mVisible = v; }

	private:
		bool mVisible;

	};

}