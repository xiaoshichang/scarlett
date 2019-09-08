#pragma once
#include "Runtime/RHI/RenderMesh.h"


namespace scarlett {
	class RenderMeshD11 : public RenderMesh {
	public:
		virtual void Render(GraphicsManager* mgr, World* world) noexcept;
	};

}