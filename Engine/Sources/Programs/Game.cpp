#include <iostream>
#include "Foundation/Platform.h"
#include "EngineConfig.h"
#include "Foundation/BuildType.h"
#include "Foundation/Archtect.h"
#include "Foundation/Assert.h"
#include "Runtime/Core/Application/WindowsApplication.h"

using namespace std;

int main() {
	cout << "EngineVersion:" << Engine_Version_Major << "." << Engine_Version_Minor << endl;
	cout << "EngineArch:" << SCARLETT_ARCH << endl;
	cout << "EngineTargetPlatform:" << SCARLETT_PLATFORM << endl;
	cout << "EngineBuildType:" << BUILD_TYPE << endl;
	
#if defined(SCARLETT_WINDOWS)
	cout << "creating windows..." << endl;
	scarlett::WindowsApplication app;
	CHECK_APPLICATION_INIT(app.Initialize());
	app.Run();
	app.Finalize();
	
#elif defined(SCARLETT_MACOS)

#endif
	return 0;
}