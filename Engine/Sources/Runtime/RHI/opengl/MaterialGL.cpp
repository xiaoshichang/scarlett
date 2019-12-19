#include "Runtime/RHI/opengl/MaterialGL.h"

void scarlett::MaterialGL::Apply(ConstantBuffer cb) noexcept
{
	mShader->Use();

	for (auto pair : mParameters) {
		if (pair.first == "color") {
			cb.debugColor = pair.second;
		}
	}
	mShader->SetConstantBuffer(cb);
}

void scarlett::MaterialGL::ApplyLight(ConstantBufferLighting cb) noexcept
{
}
