#include "MemoryManager.h"
#include <iostream>

extern "C" void* malloc(size_t size);
extern "C" void  free(void* p);

using namespace scarlett;

namespace scarlett {
	static const  uint32_t kBlockSizes[] = {
		// 4-increments
		4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
		52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,
		// 32-increments
		128, 160, 192, 224, 256, 288, 320, 352, 384,
		416, 448, 480, 512, 544, 576, 608, 640,
		// 64-increments
		704, 768, 832, 896, 960, 1024
	};
	// number of elements in the block size array
	static const uint32_t kNumBlockSizes =
		sizeof(kBlockSizes) / sizeof(kBlockSizes[0]);
	// largest valid block size
	static const uint32_t kMaxBlockSize =
		kBlockSizes[kNumBlockSizes - 1];

	static const uint32_t kPageSize = 8192;
	static const uint32_t kAlignment = 4;
	
	size_t*        MemoryManager::m_pBlockSizeLookup = nullptr;
	Allocator*     MemoryManager::m_pAllocators = nullptr;

}

scarlett::MemoryManager::MemoryManager()
{
	Initialize();
}

scarlett::MemoryManager::~MemoryManager()
{
	Finalize();
}

int MemoryManager::Initialize() noexcept {
	static bool s_bInitialized = false;
	if (!s_bInitialized) {
		m_pBlockSizeLookup = new size_t[kMaxBlockSize + 1];
		size_t j = 0;
		for (size_t i = 0; i <= kMaxBlockSize; ++i) {
			if (i > kBlockSizes[j]) ++j;
			m_pBlockSizeLookup[i] = j;
		}

		m_pAllocators = new Allocator[kNumBlockSizes];
		for (size_t i = 0; i < kNumBlockSizes; ++i) {
			m_pAllocators[i].Reset(kBlockSizes[i], kPageSize, kAlignment);
		}

		s_bInitialized = true;
	}
	return 0;
}

void MemoryManager::Finalize() noexcept{
	delete[] m_pBlockSizeLookup;
	delete[] m_pAllocators;
}

Allocator* MemoryManager::LookupAllocator(size_t size) {
	if (size <= kMaxBlockSize) {
		return m_pAllocators + m_pBlockSizeLookup[size];
	}
	else {
		return nullptr;
	}
}

void* MemoryManager::Allocate(size_t size) {
	Allocator* pAlloc = LookupAllocator(size);
	if (pAlloc)
		return pAlloc->Allocate();
	else
		return malloc(size);
}

void MemoryManager::Free(void* p, size_t size) {
	Allocator* pAlloc = LookupAllocator(size);
	if (pAlloc)
		pAlloc->Free(p);
	else
		free(p);
}