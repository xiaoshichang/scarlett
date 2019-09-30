#include "Director.h"
#include "AutoReleasePool.h"
#include "Sprite.h"
#include "Listener.h"

#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Application/GlobalConfig.h"
using namespace scarlett;


Director::Director() {
}

Director::~Director() {
}

int Director::Initialize() noexcept {
	auto width = GlobalConfig::GetInstance()->GetScreenWidth();
	auto height = GlobalConfig::GetInstance()->GetScreenHeight();
	SetResolution(Vector2f(width, height));

	mDispatcher = new UIEventDispatch();

	mRoot = UINode::Create();
	mRoot->Retain();
	mRoot->SetPosition(0, 0);
	
	auto sprite = Sprite::Create("./Asset/Textures/ibl_brdf_lut.png");
	sprite->SetPosition(-512 + 51.2 + 10, -384 + 38.4 + 10);
	mRoot->AddChild(sprite);

	auto listener = UIEventListenerKeyboard::create();
	listener->onKeyPressed = [=](unsigned char code, UIEvent* event) {
		if (code == 'D') {
			auto pos = sprite->GetPosition();
			sprite->SetPosition(pos.x() + 10, pos.y());
		}
		else if (code == 'A') {
			auto pos = sprite->GetPosition();
			sprite->SetPosition(pos.x() - 10, pos.y());
		}
		else if (code == 'W') {
			auto pos = sprite->GetPosition();
			sprite->SetPosition(pos.x(), pos.y() + 10);
		}
		else if (code == 'S') {
			auto pos = sprite->GetPosition();
			sprite->SetPosition(pos.x(), pos.y() - 10);
		}
	};

	listener->onKeyReleased = [=](unsigned char code, UIEvent* event) {
		if (code == 0x1b) {
			GApp->Quit();
		}
		return;
	};
	listener->setTarget(sprite);
	mDispatcher->addEventListener(listener);

	m_Render = new Renderer();
	return 0;
}

void Director::Tick() noexcept {
}

void Director::PostTick(){
	AutoreleasePool::GetInstance()->Clear();
}

void Director::Finalize()noexcept {
}


void Director::OnRender() {
	// draw the queue with graphicsManager
	mRoot->Visit();
}

void Director::RenderQueue() {
}


Director* Director::sharedDirector = nullptr;

Director* Director::GetInstance() {
	if (sharedDirector == nullptr) {
		sharedDirector = new Director();
	}
	return sharedDirector;
}

Renderer* Director::GetRender() {
	return m_Render;
}

Vector2f Director::GetResolution() {
	return m_Resolution;
}

void Director::SetResolution(const Vector2f& resolution) {
	m_Resolution = resolution;
}

const Matrix4f Director::GetMVPMatrix() {
	Matrix4f mvp = Matrix4f::Identity();
	return mvp;
}