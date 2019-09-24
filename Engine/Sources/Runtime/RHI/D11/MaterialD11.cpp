#include "MaterialD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/RHI/D11/TextureD11.h"
#include "Runtime/RHI/D11/SamplerStateD11.h"
#include "Runtime/RHI/D11/DepthStencilStateD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"

scarlett::MaterialD11::MaterialD11()
{
	mDepthStencilState = std::make_shared<DepthStencilStateD11>();
}

void scarlett::MaterialD11::Apply(ConstantBuffer cb) noexcept
{
	if (mName == "skybox") {
		ApplySkybox(cb);
	}
	else {
		ApplySurface(cb);
	}
}

void scarlett::MaterialD11::ApplySurface(ConstantBuffer cb)
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mShader->Use();
	for (auto pair : mParameters) {
		if (pair.first == "color") {
			cb.debugColor = pair.second;
		}
	}
	mShader->SetConstantBuffer(cb);

	for (auto pair : mTextures) {
		if (pair.first == "tBaseMap") {
			auto texture = pair.second;
			auto sampler = mSamplerState[pair.first];
			if (texture && sampler) {
				auto _texuter = dynamic_pointer_cast<TextureD11>(texture);
				auto _sampler = dynamic_pointer_cast<SamplerStateD11>(sampler);
				mgrd11->m_deviceContext->PSSetShaderResources(0, 1, &(_texuter->mView));
				mgrd11->m_deviceContext->PSSetSamplers(0, 1, &(_sampler->m_sampleState));
			}
		}
	}
	auto _depth = dynamic_pointer_cast<DepthStencilStateD11>(mDepthStencilState);
	mgrd11->m_deviceContext->OMSetDepthStencilState(_depth->mDepthStencilState, 0);
}

void scarlett::MaterialD11::ApplySkybox(ConstantBuffer cb)
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mShader->Use();
	mShader->SetConstantBuffer(cb);
	for (auto pair : mTextures) {
		if (pair.first == "skybox") {
			auto texture = pair.second;
			auto sampler = mSamplerState[pair.first];
			if (texture && sampler) {
				auto _texuter = dynamic_pointer_cast<TextureD11>(texture);
				auto _sampler = dynamic_pointer_cast<SamplerStateD11>(sampler);
				mgrd11->m_deviceContext->PSSetShaderResources(0, 1, &(_texuter->mView));
				mgrd11->m_deviceContext->PSSetSamplers(0, 1, &(_sampler->m_sampleState));
			}
		}
	}

	auto _depth = dynamic_pointer_cast<DepthStencilStateD11>(mDepthStencilState);
	mgrd11->m_deviceContext->OMSetDepthStencilState(_depth->mDepthStencilState, 0);
}
