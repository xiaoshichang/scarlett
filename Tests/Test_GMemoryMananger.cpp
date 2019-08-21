#include "Runtime/Core/Memory/MemoryManager.h"
#include <iostream>

using namespace scarlett;
using namespace std;

int main() {
	auto pModule = GMemoryMananger::GetInstance();
	pModule->Initialize();

	auto pData1 = (unsigned char*)pModule->Allocate(19);
	auto pData2 = pModule->Allocate(29);
	auto pData3 = pModule->Allocate(129);

	for (int i = 0; i < 10; i++) {
		cout << unsigned int(pData1[i]) << ' ';
	}

	pModule->Free(pData1, 19);
	pModule->Free(pData2, 29);
	pModule->Free(pData3, 129);

	void* data[2048];

	for (int i = 0; i < 1024; i++) {
		data[i] = pModule->Allocate(i);
	}
	for (int i = 0; i < 1024; i++) {
		data[i+1024] = pModule->Allocate(10);
	}
	for (int i = 0; i < 1024; i++) {
		pModule->Free(data[i], i);
	}
	for (int i = 0; i < 1024; i++) {
		pModule->Free(data[i+1024], 10);
	}
	
	pModule->Finalize();
	return 0;
}