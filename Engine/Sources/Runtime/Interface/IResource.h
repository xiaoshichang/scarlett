#pragma once
#include "Runtime/Interface/IModule.h"

namespace scarlett {
	class GraphicsManager;
	class IRenderResource{
	};

	enum VertexFormat
	{
		None = 0,
		VF_P3F,
		VF_T2F,
		VF_N3F,
		VF_C4B,
	};

	class IVertexBuffer : public IRenderResource {
		virtual void Initialize(GraphicsManager* mgr, void* data, unsigned int count, VertexFormat vf) noexcept = 0;
		virtual size_t GetVertexSize(VertexFormat vf) noexcept = 0;
	};

	class IMesh : public IRenderResource {
	};

}