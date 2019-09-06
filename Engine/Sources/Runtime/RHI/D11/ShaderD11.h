#pragma once

#include "Runtime/RHI/Shader.h"
#include <d3d11.h>


namespace scarlett {
	class GraphicsManager;
	class ShaderD11 : public Shader {
	public:
		virtual	bool	InitializeFromFile(GraphicsManager* mgr, const string& vsPath, const string& psPath) noexcept;
		virtual	void	Use(GraphicsManager* mgr) noexcept;

	public:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
	};

}