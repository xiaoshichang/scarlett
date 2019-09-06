#include "Runtime/RHI/D11/ShaderD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/RHI/GraphicsMgr.h"
#include "Foundation/Assert.h"
#include <d3dcompiler.h>
#include <iostream>

bool scarlett::ShaderD11::InitializeFromFile(GraphicsManager* mgr, const string & vsPath, const string & psPath) noexcept
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	std::wstring _vsPath = std::wstring(vsPath.begin(), vsPath.end());
	std::wstring _psPath = std::wstring(psPath.begin(), psPath.end());

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

	result = D3DCompileFromFile(_vsPath.c_str(), NULL, NULL, "main", "vs_5_0", dwShaderFlags, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));
		std::cout << reinterpret_cast<const char*>(errorMessage->GetBufferPointer()) << endl;
		SCARLETT_ASSERT(false);
	}

	result = D3DCompileFromFile(_psPath.c_str(), NULL, NULL, "main", "ps_5_0", dwShaderFlags, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));
		std::cout << reinterpret_cast<const char*>(errorMessage->GetBufferPointer()) << endl;
		SCARLETT_ASSERT(false);
	}

	return true;
}

void scarlett::ShaderD11::Use(GraphicsManager* mgr) noexcept
{
}
