//
// Created by gzxiaoshichang on 2019/2/27.
//

#ifndef GAMEENGINEX_ALLOCATOR_H
#define GAMEENGINEX_ALLOCATOR_H

#include "Foundation/BuildType.h"

#include <stddef.h>
#include <stdint.h>


#ifndef ALIGN
#define ALIGN(x, a)         (((x) + ((a) - 1)) & ~((a) - 1))
#endif

namespace scarlett {

	struct BlockHeader{
		BlockHeader* next;
	};

	struct PageHeader {
		PageHeader* next;
		BlockHeader* GetBlocks() {
			return reinterpret_cast<BlockHeader*>(this + 1);
		}
	};

    /**
     * @brief a page-block allocator
     */
	class Allocator {
	public:

		static const uint8_t PATTERN_ALIGN = 0xFC;	//	252
		static const uint8_t PATTERN_ALLOC = 0xFD;	//	253
		static const uint8_t PATTERN_FREE = 0xFE;	//	254

	    /**
	     * @brief default ctor
	     */
		Allocator();

		/**
		 * @brief ctor
		 * @param dataSize
		 * @param pageSize
		 * @param alignment
		 */
		Allocator(size_t dataSize, size_t pageSize, size_t alignment);

		/**
		 * default dtor
		 */
		~Allocator();

		/**
		 * @brief reset configuration
		 * @param dataSize
		 * @param pageSize
		 * @param alignment
		 */
		void Reset(size_t dataSize, size_t pageSize, size_t alignment);


		/**
		 * @brief try to allocate memory
		 * @return address of allocated memory
		 */
		void* Allocate();

		/**
		 * @brief free memory of a block
		 * @param p
		 */
		void Free(void* p);

		/**
		 * free all memory
		 */
		void FreeAll();

	private:

#if defined(SCARLETT_DEBUG)
		void DebugFillFreePage(PageHeader* pPage);
		void DebugFillFreeBlock(BlockHeader* pBlock);
		void DebugFillAllocatedBlock(BlockHeader* pBlock);

#endif

		BlockHeader* CalcNextBlock(BlockHeader* pBlock);

		PageHeader* m_pPageList;

		BlockHeader* m_pFreeList;

		size_t m_szDataSize;
		size_t m_szPageSize;
		size_t m_szAlignmentSize;
		size_t m_szBlockSize;
		size_t m_nBlockPerPage;

		size_t m_nPages;
		size_t m_nBlock;
		size_t m_nFreeBlocks;

		Allocator(const Allocator& clone) = delete;
		Allocator &operator=(const Allocator &rhs) = delete;

	};

}


#endif // GAMEENGINEX_ALLOCATOR_H