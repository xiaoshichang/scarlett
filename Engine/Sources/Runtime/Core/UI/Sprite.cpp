#include "Sprite.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Application/GlobalConfig.h"
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

void Sprite::Render(Renderer* renderer, const Matrix4x4f& transform, uint32_t flags) {
	auto config = GlobalConfig::GetInstance();

	float width = mContentSize.x;
	float height = mContentSize.y;
	Matrix4x4f world;
	BuildMatrixScale(world, width / config->GetScreenWidth(), height / config->GetScreenHeight(), 1);
	renderer->RenderSprite(mTexture, world, transform, flags);
}