#include "Runtime/RHI/opengl/MaterialGL.h"

void scarlett::MaterialGL::ApplySurface(ConstantBuffer cb)
{
	mShader->Use();

	for (auto pair : mParameters) {
		if (pair.first == "color") {
			cb.debugColor = pair.second;
		}
	}
	mShader->SetConstantBuffer(cb);
}

void scarlett::MaterialGL::ApplySkybox(ConstantBuffer cb)
{
}
