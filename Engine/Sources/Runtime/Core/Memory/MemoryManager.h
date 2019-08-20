//
// Created by gzxiaoshichang on 2019/2/27.
//

#ifndef GAMEENGINEX_MEMORYMANAGER_H
#define GAMEENGINEX_MEMORYMANAGER_H

#include "Allocator.h"
#include "Runtime/Interface/IModule.h"
#include "Runtime/Utils/Singleton.h"

namespace scarlett {

	class MemoryManager : public IModule {
	public:

		/**
		 * New operator
		 * @tparam T data type or object type
		 * @tparam Arguments
		 * @param parameters parameters of constructor
		 * @return allocated object pointer
		 */
		template<class T, typename... Arguments>
		T* New(Arguments... parameters)
		{
			return new (Allocate(sizeof(T))) T(parameters...);
		}

		/**
		 * delete operator
		 * @tparam T data type
		 * @param p pointer of memory to delete
		 */
		template<class T>
		void Delete(T* p)
		{
			p->~T();
			Free(p, sizeof(T));
		}

		virtual ~MemoryManager() = default;
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

		/**
		 * allocate size byte
		 * @param size size of memory to allocate
		 * @return
		 */
		void* Allocate(size_t size);

		/**
		 * free size of memory
		 * @param p object pointer or 
		 * @param size
		 */
		void Free(void* p, size_t size);

	private:
		MemoryManager() = default;
		MemoryManager(const MemoryManager& m) = default;
		MemoryManager& operator=(const MemoryManager&) = default;

		static size_t* m_pBlockSizeLookup;
		static Allocator * m_pAllocators;
		static Allocator* LookupAllocator(size_t size);
		
		friend class Singleton<MemoryManager>;	// to access pInstance
	};

	typedef Singleton<MemoryManager> GMemoryMananger;
}

#endif // GAMEENGINEX_MEMORYMANAGER_H