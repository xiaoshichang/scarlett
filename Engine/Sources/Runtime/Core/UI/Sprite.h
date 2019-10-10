#pragma once
#include <string>
#include "Node.h"

namespace scarlett {
	class Sprite : public UINode {
	public:
		static Sprite* Create(std::string texture);
		void SetTexture(std::string);
		std::string GetTexture();
		virtual void Render(Renderer* renderer, const Matrix4x4f& transform, uint32_t flags);

	private:
		std::string mTexture;

	};

}