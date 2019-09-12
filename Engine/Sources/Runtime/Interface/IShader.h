#pragma once
#include <string>
#include "Runtime/Core/Math/ScltMath.h"
#include <DirectXMath.h>
#include "Runtime/Interface/IResource.h"

using namespace DirectX;
using namespace std;


namespace scarlett {

	class IShader : public IRenderResource {
	public:
		virtual	bool	InitializeFromFile(const string& vsPath, const string& psPath) noexcept = 0;
		virtual	void	Use() noexcept = 0;
		virtual void	Finialize() noexcept = 0;
		virtual void	SetConstantBuffer(const ConstantBuffer& cbuffer) noexcept = 0;

	};

}