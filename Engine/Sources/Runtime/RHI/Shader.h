#pragma once

#include "Runtime/Interface/IShader.h"

namespace scarlett {

	class GraphicsManager;
	class Shader : public IShader {

	public:
		virtual	bool	InitializeFromFile(GraphicsManager* mgr, const string& vsPath, const string& psPath) noexcept;
		virtual void	Use(GraphicsManager* mgr) noexcept;
		virtual void	Finialize(GraphicsManager* mgr) noexcept;
		virtual void	SetConstantBuffer(GraphicsManager* mgr, const ConstantBuffer& cbuffer) noexcept;
	};

}