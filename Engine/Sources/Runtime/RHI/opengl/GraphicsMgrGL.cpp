#include "GraphicsMgrGL.h"
#include "Foundation/Assert.h"
#include "glad/glad_wgl.h"
#include "Runtime/RHI/opengl/RenderMeshGL.h"
#include "Runtime/RHI/opengl/VertexBufferGL.h"
#include "Runtime/RHI/opengl/ShaderGL.h"
#include "Runtime/Utils/Logging.h"

using namespace scarlett;

int scarlett::GraphicsMgrGL::Initialize() noexcept
{
	int result;
	result = gladLoadGL();
	SCARLETT_ASSERT(result);

	if (GLAD_GL_VERSION_3_0) {
		// Enable depth testing.
		glClearDepth(1.0f);
		// Enable depth testing.
		glEnable(GL_DEPTH_TEST);
	}
	glViewport(0, 0, 1024, 768);
	SCARLETT_LOG(info) << "OpenGl Version: " << GLVersion.major << "." << GLVersion.minor << " loaded";
	LoadShaders();

	wglSwapIntervalEXT(0); // disable vertical sync to get high fps.
	return 0;
}


void scarlett::GraphicsMgrGL::Finalize() noexcept
{
	
}

void scarlett::GraphicsMgrGL::Tick() noexcept
{
}

void scarlett::GraphicsMgrGL::Present() noexcept
{
	glFlush();
}

void scarlett::GraphicsMgrGL::ClearRenderTarget(float r, float g, float b, float a) noexcept
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::shared_ptr<VertexBuffer> scarlett::GraphicsMgrGL::CreateVertexBuffer(void * data, int count, VertexFormat vf) noexcept
{
	auto ptr = std::make_shared<VertexBufferGL>(data, count, vf, 0);
	return ptr;
}

std::shared_ptr<IndexBuffer> scarlett::GraphicsMgrGL::CreateIndexBuffer(void * data, int count, IndexFormat iformat) noexcept
{
	return nullptr;
}

std::shared_ptr<RenderMesh> scarlett::GraphicsMgrGL::CreateRenderMesh(aiMesh * mesh, const aiScene * world) noexcept
{
	auto ptr = std::make_shared<RenderMeshGL>(mesh, world);
	return ptr;
}

std::shared_ptr<RenderMesh> scarlett::GraphicsMgrGL::CreateRenderMeshDebug(void* data, int count, VertexFormat vf) noexcept
{
	auto ptr = std::make_shared<RenderMeshGL>(data, count, vf);
	return ptr;
}

std::shared_ptr<Texture> scarlett::GraphicsMgrGL::CreateTexture2D(const std::string & path) noexcept
{
	return std::shared_ptr<Texture>();
}

std::shared_ptr<SamplerState> scarlett::GraphicsMgrGL::CreateSamplerState() noexcept
{
	return std::shared_ptr<SamplerState>();
}

void scarlett::GraphicsMgrGL::LoadShaders() noexcept
{
	std::string pbrShaderVS = "Asset/Shaders/gl_pbr.vs";
	std::string pbrShaderPS = "Asset/Shaders/gl_pbr.ps";
	auto pbrShader = std::make_shared<ShaderGL>(pbrShaderVS, pbrShaderPS);
	mShaders["pbr"] = pbrShader;

	std::string debugShaderVS = "Asset/Shaders/gl_debug.vs";
	std::string debugShaderPS = "Asset/Shaders/gl_debug.ps";
	auto debugShader = std::make_shared<ShaderGL>(debugShaderVS, debugShaderPS);
	mShaders["debug"] = debugShader;
}


void scarlett::GraphicsMgrGL::UseShader(std::shared_ptr<Shader> shader) noexcept
{
	if (!shader) {
		SCARLETT_ASSERT(false);
	}
	shader->Use();
}

std::shared_ptr<Shader> scarlett::GraphicsMgrGL::GetShader(const std::string & shaderName) noexcept
{
	return mShaders[shaderName];
}

void scarlett::GraphicsMgrGL::Draw(unsigned int vcount, unsigned int start) noexcept
{
}

void scarlett::GraphicsMgrGL::DrawIndexed(unsigned int icount, unsigned int start, int baseLoc) noexcept
{
}
