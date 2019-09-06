#pragma once

#include "Runtime/RHI/Shader.h"
#include <d3d11.h>


namespace scarlett {
	class GraphicsMgr;
	class ShaderD11 : public Shader {
	public:
		virtual	bool	InitializeFromFile(GraphicsMgr* mgr, const string& vsPath, const string& psPath) noexcept;
		virtual	void	Use(GraphicsMgr* mgr) noexcept;

	public:

	};

}