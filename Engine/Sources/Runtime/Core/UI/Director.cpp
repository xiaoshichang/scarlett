#include "Director.h"
#include "AutoReleasePool.h"
#include "Sprite.h"
#include "Listener.h"

#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Application/GlobalConfig.h"
using namespace scarlett;


Director::Director():
	mFps(nullptr)
{
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
	
	auto sprite = Sprite::Create("./Asset/Textures/highmap.jpg");
	sprite->SetPosition(-512 + 50 + 10, -384 + 50 + 10);
	sprite->SetContentSize(100, 100);
	mRoot->AddChild(sprite);

	auto sprite1 = Sprite::Create("./Asset/Textures/terrain_low.png");
	sprite1->SetPosition(-512 + 150 + 10, -384 + 50 + 10);
	sprite1->SetContentSize(60, 60);
	mRoot->AddChild(sprite1);

	auto sprite2 = Sprite::Create("./Asset/Textures/terrain_high.png");
	sprite2->SetPosition(-512 + 220 + 10, -384 + 50 + 10);
	sprite2->SetContentSize(60, 60);
	mRoot->AddChild(sprite2);

	auto font = FontNode::Create();
	mRoot->AddChild(font);
	font->SetPosition(512 - 150, 384 - 15);
	mFps = font;

	auto listener = UIEventListenerKeyboard::create();
	listener->onKeyPressed = [=](unsigned char code, UIEvent* event) {
		if (code == 'D') {
			auto pos = sprite->GetPosition();
			sprite->SetPosition(pos.x + 10, pos.y);
		}
		else if (code == 'A') {
			auto pos = sprite->GetPosition();
			sprite->SetPosition(pos.x - 10, pos.y);
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

const Matrix4x4f Director::GetMVPMatrix() {
	Matrix4x4f mvp;
	BuildMatrixIdentity(mvp);
	return mvp;
}