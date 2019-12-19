#include "MaterialD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/RHI/D11/TextureD11.h"
#include "Runtime/RHI/D11/SamplerStateD11.h"
#include "Runtime/RHI/D11/DepthStencilStateD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/RHI/D11/BlendStateD11.h"

scarlett::MaterialD11::MaterialD11()
{
	mDepthStencilState = std::make_shared<DepthStencilStateD11>();
	mBlendState = std::make_shared<BlendStateD11>();
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

		if (pair.first == "pbrParameter") {
			cb.pbrParameter = pair.second;
		}
	}
	auto camera = GApp->mWorld->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
	cb.camPos = Vector4f(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z, 1.0f);
	mShader->SetConstantBuffer(cb);

	auto skybox = camera->GetSkybox();
	if (skybox) {
		auto _texuter = static_pointer_cast<TextureD11>(skybox->mIrradianceMap);
		auto _sampler = static_pointer_cast<SamplerStateD11>(skybox->mIrradianceMapSamplerState);
		mgrd11->m_deviceContext->PSSetShaderResources(0, 1, &(_texuter->mView));
		mgrd11->m_deviceContext->PSSetSamplers(0, 1, &(_sampler->m_sampleState));

		_texuter = static_pointer_cast<TextureD11>(skybox->mEnvmap);
		_sampler = static_pointer_cast<SamplerStateD11>(skybox->mEnvmapSamplerState);
		mgrd11->m_deviceContext->PSSetShaderResources(1, 1, &(_texuter->mView));
		mgrd11->m_deviceContext->PSSetSamplers(1, 1, &(_sampler->m_sampleState));
	}

	for (auto pair : mTextures) {
		if (pair.first == "lut") {
			auto tex = pair.second;
			auto _texuter = static_pointer_cast<TextureD11>(tex);
			mgrd11->m_deviceContext->PSSetShaderResources(2, 1, &(_texuter->mView));
		}

		else if (pair.first == "ui") {
			auto tex = pair.second;
			auto _texuter = static_pointer_cast<TextureD11>(tex);
			mgrd11->m_deviceContext->PSSetShaderResources(0, 1, &(_texuter->mView));
		}

		else if (pair.first == "terrain") {
			auto tex = pair.second;
			auto _texuter = static_pointer_cast<TextureD11>(tex);
			mgrd11->m_deviceContext->PSSetShaderResources(0, 1, &(_texuter->mView));
		}
	}

	for (auto pair : mSamplerState) {
		if (pair.first == "lut") {
			auto sampler = pair.second;
			auto _sampler = static_pointer_cast<SamplerStateD11>(sampler);
			mgrd11->m_deviceContext->PSSetSamplers(2, 1, &(_sampler->m_sampleState));
		}

		else if (pair.first == "ui") {
			auto sampler = pair.second;
			auto _sampler = static_pointer_cast<SamplerStateD11>(sampler);
			mgrd11->m_deviceContext->PSSetSamplers(0, 1, &(_sampler->m_sampleState));
		}

		else if (pair.first == "terrain") {
			auto sampler = pair.second;
			auto _sampler = static_pointer_cast<SamplerStateD11>(sampler);
			mgrd11->m_deviceContext->PSSetSamplers(0, 1, &(_sampler->m_sampleState));
		}
	}


	auto _depth = static_pointer_cast<DepthStencilStateD11>(mDepthStencilState);
	mgrd11->m_deviceContext->OMSetDepthStencilState(_depth->mDepthStencilState, 0);

	auto _blend = static_pointer_cast<BlendStateD11>(mBlendState);
	float blendFactor[] = { 0.f,0.f,0.f,0.f };
	mgrd11->m_deviceContext->OMSetBlendState(_blend->mState, 0, 0xffffffff);
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
				auto _texuter = static_pointer_cast<TextureD11>(texture);
				auto _sampler = static_pointer_cast<SamplerStateD11>(sampler);
				mgrd11->m_deviceContext->PSSetShaderResources(0, 1, &(_texuter->mView));
				mgrd11->m_deviceContext->PSSetSamplers(0, 1, &(_sampler->m_sampleState));
			}
		}
	}

	auto _depth = static_pointer_cast<DepthStencilStateD11>(mDepthStencilState);
	mgrd11->m_deviceContext->OMSetDepthStencilState(_depth->mDepthStencilState, 0);

	auto _blend = static_pointer_cast<BlendStateD11>(mBlendState);
	float blendFactor[] = { 0.f,0.f,0.f,0.f };
	mgrd11->m_deviceContext->OMSetBlendState(_blend->mState, blendFactor, 0xffffffff);
}

void scarlett::MaterialD11::ApplyLight(ConstantBufferLighting cb) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mShader->Use();
	mShader->SetConstantBufferLight(cb);


}
