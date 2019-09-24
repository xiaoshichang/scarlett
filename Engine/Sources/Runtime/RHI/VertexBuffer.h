#pragma once

#include "Runtime/Interface/IResource.h"

namespace scarlett {

	enum VertexFormat
	{
		VF_None = 0,
		VF_P3F,
		VF_T2F,
		VF_N3F,
		VF_C4B,
	};

	class IVertexBuffer : public IRenderResource {
	public:
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf) noexcept = 0;

		IVertexBuffer() : mVertexCount(0), mVertexFormat(VertexFormat::VF_None) {}

		unsigned int GetVertexSize(VertexFormat vf) {
			if (vf == VertexFormat::VF_P3F || vf == VertexFormat::VF_N3F) {
				return sizeof(float) * 3;
			}
			else if (vf == VertexFormat::VF_C4B) {
				return 4;
			}
			else if (vf == VertexFormat::VF_T2F) {
				return sizeof(float) * 2;
			}
			else {
				return 0;
			}
		}

		unsigned int GetVertextCount() { return mVertexCount; }

	public:
		unsigned int mVertexCount;
		VertexFormat mVertexFormat;
	};
	
}