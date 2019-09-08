#pragma once
#include <string>
#include "Runtime/Core/Math/ScltMath.h"
#include <DirectXMath.h>
using namespace DirectX;
using namespace std;


namespace scarlett {

	struct ConstantBuffer
	{
		Matrix4f world;
		Matrix4f view;
		Matrix4f projection;
	};

	class GraphicsManager;
	class IShader {
	public:
		virtual	bool	InitializeFromFile(GraphicsManager* mgr, const string& vsPath, const string& psPath) noexcept = 0;
		virtual	void	Use(GraphicsManager* mgr) noexcept = 0;
		virtual void	Finialize(GraphicsManager* mgr) noexcept = 0;
		virtual void	SetConstantBuffer(GraphicsManager* mgr, const ConstantBuffer& cbuffer) noexcept = 0;

	};

}