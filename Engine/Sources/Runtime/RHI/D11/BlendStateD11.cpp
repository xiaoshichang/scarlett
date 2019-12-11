#include "BlendStateD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"


scarlett::BlendStateD11::BlendStateD11()
{
	SetEnable(false);
}

scarlett::BlendStateD11::~BlendStateD11()
{
}

void scarlett::BlendStateD11::SetEnable(bool enable)
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));

	if (enable) {
		BlendState.RenderTarget[0].BlendEnable = TRUE;
		BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	}
	else {
		BlendState.RenderTarget[0].BlendEnable = FALSE;
		BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	}
	
	auto res = mgrd11->m_device->CreateBlendState(&BlendState, &mState);
	if (FAILED(res)) {
		SCARLETT_ASSERT(false);
	}
	mEnable = enable;
}
