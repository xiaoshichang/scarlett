#include "IndexBufferD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Foundation/Assert.h"

scarlett::IndexBufferD11::IndexBufferD11(void* data, unsigned int count, IndexFormat iformat)
{
	mIndexCount = count;
	mIndexFormat = iformat;
	Initialize(data, count, iformat);
}

scarlett::IndexBufferD11::~IndexBufferD11()
{
	if (mIndexBuffer) {
		mIndexBuffer->Release();
	}
}

void scarlett::IndexBufferD11::Initialize(void * data, unsigned int count, IndexFormat iformat) noexcept
{
	HRESULT result;
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = data;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = mgrd11->m_device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(result)){
		SCARLETT_ASSERT(false);
	}
}
