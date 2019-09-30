#include <iostream>
#include "Foundation/Platform.h"
#include "EngineConfig.h"
#include "Foundation/BuildType.h"
#include "Foundation/Archtect.h"
#include "Foundation/Assert.h"
#include "Runtime/Core/Application/WindowsApplication.h"
#include "Runtime/Core/Application/AssetFinder.h"
#include "Runtime/Utils/Logging.h"

namespace scarlett {
#if defined(SCARLETT_WINDOWS)
	extern Application* GApp = new WindowsApplication();
#elif defined(SCARLETT_MACOS)

#endif
}

using namespace std;
using namespace scarlett;

int main() {
	SCARLETT_LOG(info) << "EngineVersion:" << Engine_Version_Major << "." << Engine_Version_Minor;
	SCARLETT_LOG(info) << "EngineArch:" << SCARLETT_ARCH;
	SCARLETT_LOG(info) << "EngineTargetPlatform:" << SCARLETT_PLATFORM;
	SCARLETT_LOG(info) << "EngineBuildType:" << BUILD_TYPE;
	
#if defined(SCARLETT_WINDOWS)
	CHECK_APPLICATION_INIT(GApp->Initialize());
	auto scene = GAssetFinder::GetInstance()->GetRealPath("Scenes/robot-v2.fbx");
	GApp->mWorld->LoadScene(scene);
	GApp->mWorld->DumpEntities();
	GApp->mWorld->GetRenderDebugSystem()->CreateDebugMesh();
	GApp->mWorld->GetRenderDebugSystem()->CreateDebugAxis();

	GApp->Run();
	GApp->Finalize();
	
#elif defined(SCARLETT_MACOS)

#endif
	return 0;
}