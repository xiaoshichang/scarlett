#include "Runtime/RHI/D11/ShaderD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/RHI/GraphicsMgr.h"
#include "Runtime/Core/Application/Application.h"
#include "Foundation/Assert.h"
#include <d3dcompiler.h>
#include <iostream>

scarlett::ShaderD11::ShaderD11(const string & vsPath, const string & psPath)
{
	InitializeFromFile(vsPath, psPath);
}

scarlett::ShaderD11::~ShaderD11()
{
	Finialize();
}

bool scarlett::ShaderD11::InitializeFromFile(const string & vsPath, const string & psPath) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;

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

	result = D3DCompileFromFile(_psPath.c_str(), NULL, NULL, "main", "ps_5_0", dwShaderFlags, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));
		std::cout << reinterpret_cast<const char*>(errorMessage->GetBufferPointer()) << endl;
		SCARLETT_ASSERT(false);
	}
	result = mgrd11->m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = mgrd11->m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}


	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 1;
	polygonLayout[1].AlignedByteOffset = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOOD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 2;
	polygonLayout[2].AlignedByteOffset = 0;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	unsigned int numElements;
	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	// Create the vertex input layout.
	auto size = vertexShaderBuffer->GetBufferSize();
	auto pointer = vertexShaderBuffer->GetBufferPointer();
	result = mgrd11->m_device->CreateInputLayout(polygonLayout, numElements, pointer, size, &m_layout);

	if (FAILED(result)){
		SCARLETT_ASSERT(false);
	}

	D3D11_BUFFER_DESC matrixBufferDesc;
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = mgrd11->m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)){
		SCARLETT_ASSERT(false);
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}

void scarlett::ShaderD11::Use() noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	// Set the vertex input layout.
	mgrd11->m_deviceContext->IASetInputLayout(m_layout);
	// Set the vertex and pixel shaders that will be used to render this triangle.
	mgrd11->m_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	mgrd11->m_deviceContext->PSSetShader(m_pixelShader, NULL, 0);
}

void scarlett::ShaderD11::Finialize() noexcept
{
	// Release the matrix constant buffer.
	if (m_matrixBuffer){
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if (m_layout){
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader){
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader){
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void scarlett::ShaderD11::SetConstantBuffer(const ConstantBuffer & cbuffer) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstantBuffer* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = mgrd11->m_deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)){
		SCARLETT_ASSERT(false);
	}

	dataPtr = (ConstantBuffer*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	dataPtr->world = cbuffer.world;
	dataPtr->view = cbuffer.view;
	dataPtr->projection = cbuffer.projection;
	dataPtr->debugColor = cbuffer.debugColor;
	// Unlock the constant buffer.
	mgrd11->m_deviceContext->Unmap(m_matrixBuffer, 0);
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;
	mgrd11->m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	mgrd11->m_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
}
