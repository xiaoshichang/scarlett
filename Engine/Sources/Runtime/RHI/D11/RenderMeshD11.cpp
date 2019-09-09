#include "Runtime/RHI/D11/RenderMeshD11.h"
#include "Runtime/RHI/D11/IndexBufferD11.h"
#include "Runtime/RHI/D11/VertexBufferD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Object/World.h"

#include <DirectXMath.h>
using namespace DirectX;

void scarlett::RenderMeshD11::Render(GraphicsManager * mgr, World* world) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)mgr;
	int vbcount = GetVaildVertexBufferCount();

	unsigned int* stride = new  unsigned int[vbcount];
	unsigned int* offset = new  unsigned int[vbcount];
	ID3D11Buffer ** vbuffers = new ID3D11Buffer *[vbcount];
	VertexBufferD11* buffer;
	int idx = 0;
	if (mPositions) {
		stride[idx] = mPositions->GetVertexSize(mPositions->mVertexFormat);
		offset[idx] = 0;
		buffer = (VertexBufferD11*)mPositions.get();
		vbuffers[idx] = buffer->mVertexBuffer;
		idx += 1;
	}
	if (mNormals) {
		stride[idx] = mNormals->GetVertexSize(mNormals->mVertexFormat);
		offset[idx] = 0;
		buffer = (VertexBufferD11*)mNormals.get();
		vbuffers[idx] = buffer->mVertexBuffer;
		idx += 1;
	}
	if (mTexCoords) {
		stride[idx] = mTexCoords->GetVertexSize(mTexCoords->mVertexFormat);
		offset[idx] = 0;
		buffer = (VertexBufferD11*)mTexCoords.get();
		vbuffers[idx] = buffer->mVertexBuffer;
		idx += 1;
	}
	mgrd11->m_deviceContext->IASetVertexBuffers(0, vbcount, vbuffers, stride, offset);


	// Set the index buffer to active in the input assembler so it can be rendered.
	if (mIndexes) {
		auto indexBuffer = static_pointer_cast<IndexBufferD11>(mIndexes);
		mgrd11->m_deviceContext->IASetIndexBuffer(indexBuffer->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	if (mType == PrimitiveType::PT_LINE) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	}
	else if (mType == PrimitiveType::PT_POINT) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	}
	else if (mType == PrimitiveType::PT_TRIANGLE) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	delete stride;
	delete offset;
	delete vbuffers;
	
	auto shader = mgrd11->UseShader("debug");
	auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();

	ConstantBuffer cb;
	cb.world = Matrix4f::Identity();
	cb.view = camera->GetViewMatrix().transpose();
	cb.projection = camera->GetPerspectiveMatrix().transpose();
	shader->SetConstantBuffer(mgrd11, cb);

	if (mIndexes) {
		mgrd11->DrawIndexed(mIndexes->mCount, 0, 0);
	}
	else {
		mgrd11->Draw(mPositions->mCount, 0);
	}

}
