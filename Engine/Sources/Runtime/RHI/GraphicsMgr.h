#pragma once

#include "Runtime/Interface/IModule.h"
#include "Runtime/Utils/Singleton.h"

namespace scarlett{

	class GraphicsManager : public ITickableModule{
	public:
		virtual void Render() noexcept = 0;
		virtual void ClearRenderTarget(float r, float g, float b, float a) noexcept = 0;
		virtual void CreateDeviceContext() noexcept = 0;
		virtual void ReleaseDeviceContext() noexcept = 0;

	};

}


