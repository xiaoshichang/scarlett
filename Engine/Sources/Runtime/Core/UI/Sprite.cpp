#include "Sprite.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Utils/Logging.h"

using namespace scarlett;


Sprite* Sprite::Create(std::string texture) {
	auto node = new Sprite();
	node->SetTexture(texture);
	node->AutoRelease();
	return node;
}

void Sprite::SetTexture(std::string tex) {
	mTexture = tex;
}

std::string Sprite::GetTexture() {
	return mTexture;
}

void Sprite::Render(Renderer* renderer, const Matrix4f& transform, uint32_t flags) {

	float width = 102.4f;
	float height = 76.8f;
	auto world = BuildScaleMatrix(Vector3f(width / 2 / 512, height / 2 / 384, 1));

	renderer->RenderSprite(mTexture, world, transform, flags);
}