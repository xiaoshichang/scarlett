#include "DepthStencilStateD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"

using namespace scarlett;

scarlett::DepthStencilStateD11::DepthStencilStateD11(): 
	mMask(DepthStencilStateWriteMask::ALL),
	mFunc(DepthStencilStateFunc::LESS),
	mDepthStencilState(nullptr)
{
	SafeRelease();
	Reload();
}

void scarlett::DepthStencilStateD11::SetWriteMask(DepthStencilStateWriteMask mask)
{
	mMask = mask;
	SafeRelease();
	Reload();
}

void scarlett::DepthStencilStateD11::SetFunc(DepthStencilStateFunc func)
{
	mFunc = func;
	SafeRelease();
	Reload();
}

void scarlett::DepthStencilStateD11::SafeRelease()
{
	if (mDepthStencilState) {
		mDepthStencilState->Release();
	}
}

void scarlett::DepthStencilStateD11::Reload()
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;

	if (mMask == DepthStencilStateWriteMask::ALL) {
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	}
	else {
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	}

	if (mFunc == DepthStencilStateFunc::ALWAYS) {
		dssDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	}
	else if(mFunc == DepthStencilStateFunc::LESS_EQUAL){
		dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	}
	else if(mFunc == DepthStencilStateFunc::LESS){
		dssDesc.DepthFunc = D3D11_COMPARISON_LESS;
	}
	else if (mFunc == DepthStencilStateFunc::GREATER) {
		dssDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	}
	else if (mFunc == DepthStencilStateFunc::GREATER_EQUAL) {
		dssDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	}
	else if (mFunc == DepthStencilStateFunc::EQUAL) {
		dssDesc.DepthFunc = D3D11_COMPARISON_EQUAL;
	}
	else if (mFunc == DepthStencilStateFunc::NOT_EQUAL) {
		dssDesc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
	}
	else{
		dssDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	}
	mgrd11->m_device->CreateDepthStencilState(&dssDesc, &mDepthStencilState);
}
