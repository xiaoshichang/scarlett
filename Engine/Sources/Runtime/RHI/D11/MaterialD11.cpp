#include "MaterialD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/RHI/D11/TextureD11.h"
#include "Runtime/RHI/D11/SamplerStateD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"


void scarlett::MaterialD11::Apply(ConstantBuffer cb) noexcept
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
}
