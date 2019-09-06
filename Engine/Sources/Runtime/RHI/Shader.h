#pragma once

#include "Runtime/Interface/IShader.h"

namespace scarlett {
	class GraphicsMgr;
	class Shader : public IShader {

	public:
		virtual	bool	InitializeFromFile(GraphicsMgr* mgr, const string& vsPath, const string& psPath) noexcept;
		virtual void	Use(GraphicsMgr* mgr) noexcept;
	};

}