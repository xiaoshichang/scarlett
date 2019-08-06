#include <iostream>
#include "Foundation/Platform.h"
#include "EngineConfig.h"
#include "Foundation/BuildType.h"
#include "Foundation/Archtect.h"
#include "Foundation/Assert.h"

using namespace std;

int main() {
	cout << "EngineVersion:" << Engine_Version_Major << "." << Engine_Version_Minor << endl;
	cout << "EngineArch:" << SCARLETT_ARCH << endl;
	cout << "EngineTargetPlatform:" << SCARLETT_PLATFORM << endl;
	cout << "EngineBuildType:" << BUILD_TYPE << endl;
	
	int a, b;
	a = 1, b = 2;
	SCARLETT_ASSERT(a > b);

	return 0;
}