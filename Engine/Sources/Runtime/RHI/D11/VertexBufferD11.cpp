#include "Runtime/RHI/D11/VertexBufferD11.h"
#include "Runtime/RHI/GraphicsMgr.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Foundation/Assert.h"

#include <d3d11.h>
using namespace scarlett;

void scarlett::VertexBufferD11::Initialize(GraphicsManager * mgr, void * data, unsigned int count, VertexFormat vf) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)mgr;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = this->GetVertexSize(vf) * count;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = data;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	auto result = mgrd11->m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &(this->mVertexBuffer));
}
