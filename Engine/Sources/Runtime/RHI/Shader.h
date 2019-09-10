#pragma once

#include "Runtime/Interface/IShader.h"

namespace scarlett {

	class Shader : public IShader {

	public:
		virtual	bool	InitializeFromFile(const string& vsPath, const string& psPath) noexcept;
		virtual void	Use() noexcept;
		virtual void	Finialize() noexcept;
		virtual void	SetConstantBuffer(const ConstantBuffer& cbuffer) noexcept;
	};

}