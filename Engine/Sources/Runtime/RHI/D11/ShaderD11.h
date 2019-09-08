#pragma once

#include "Runtime/RHI/Shader.h"
#include <d3d11.h>


namespace scarlett {
	class GraphicsManager;
	class ShaderD11 : public Shader {
	public:
		virtual	bool	InitializeFromFile(GraphicsManager* mgr, const string& vsPath, const string& psPath) noexcept;
		virtual	void	Use(GraphicsManager* mgr) noexcept;
		virtual void	Finialize(GraphicsManager* mgr) noexcept;
		virtual void	SetConstantBuffer(GraphicsManager* mgr, const ConstantBuffer& cbuffer) noexcept;

	public:
		ID3D11VertexShader*		m_vertexShader;
		ID3D11PixelShader*		m_pixelShader;
		ID3D11InputLayout*		m_layout;
		ID3D11Buffer*			m_matrixBuffer;

	};

}