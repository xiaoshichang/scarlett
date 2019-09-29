#pragma once
#include <unordered_map>
#include "Sprite.h"
#include "Runtime/RHI/Mesh.h"

namespace scarlett {

	class Renderer {
	public:
		Renderer();
		virtual ~Renderer(){}

	public:
		void RenderSprite(const std::string& texture, const Matrix4f & world, const Matrix4f& transform, uint32_t flags);

	private:

		std::shared_ptr<IMesh>	mQuad;

	};

}