#include <iostream>
#include "Foundation/Platform.h"
#include "EngineConfig.h"
#include "Foundation/BuildType.h"
#include "Foundation/Archtect.h"
#include "Foundation/Assert.h"
#include "Runtime/Core/Application/WindowsApplication.h"


using namespace std;
using namespace scarlett;

#if defined(SCARLETT_WINDOWS)
extern Application* GApp = GWindowsApplication::GetInstance();
#elif defined(SCARLETT_MACOS)

#endif


int main() {
	cout << "EngineVersion:" << Engine_Version_Major << "." << Engine_Version_Minor << endl;
	cout << "EngineArch:" << SCARLETT_ARCH << endl;
	cout << "EngineTargetPlatform:" << SCARLETT_PLATFORM << endl;
	cout << "EngineBuildType:" << BUILD_TYPE << endl;
	
#if defined(SCARLETT_WINDOWS)
	CHECK_APPLICATION_INIT(GApp->Initialize());
	GApp->mWorld->LoadScene("Asset/Scenes/aili_cycle.fbx");
	GApp->mWorld->DumpEntities();

	GApp->Run();
	GApp->Finalize();
	
#elif defined(SCARLETT_MACOS)

#endif
	return 0;
}