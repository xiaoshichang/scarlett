#pragma once
#include "Runtime/Interface/IResource.h"
#include <string>

namespace scarlett {

	class IShader : public IRenderResource {
	public:
		virtual	bool	InitializeFromFile(const std::string& vsPath, const std::string& psPath) noexcept = 0;
		virtual	void	Use() noexcept = 0;
		virtual void	SetConstantBuffer(const ConstantBuffer& cbuffer) noexcept = 0;
		virtual void	SetConstantBufferLight(const ConstantBufferLighting& cbuffer) noexcept = 0;

	};
}