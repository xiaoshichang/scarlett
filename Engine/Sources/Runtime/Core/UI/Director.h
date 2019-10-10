#pragma once
#include "Node.h"
#include "Renderer.h"
#include "Dispatcher.h"
#include "Runtime/Interface/IModule.h"
#include "Runtime/Core/Math/ScltMath.h"

namespace scarlett {

	class Director : public ITickableModule {
	public:
		static Director* GetInstance();
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;
		void PostTick();

	protected:
		Director();

	public:
		virtual ~Director();
		
		void OnRender();
		void RenderQueue();

		Renderer* GetRender();
		Vector2f GetResolution();
		void SetResolution(const Vector2f& resolution);

		const Matrix4x4f GetMVPMatrix();

		UIEventDispatch* GetDispatcher() { return mDispatcher; }

	private:
		static Director* sharedDirector;
		Renderer* m_Render;
		Vector2f m_Resolution;

		UINode* mRoot;
		UIEventDispatch* mDispatcher;

		
	};


}