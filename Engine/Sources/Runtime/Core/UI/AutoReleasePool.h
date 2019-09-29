#pragma once

#include <vector>
#include <string>
#include "Ref.h"

namespace scarlett {
	class AutoreleasePool {
	public:
		static AutoreleasePool* GetInstance();

		AutoreleasePool();
		virtual ~AutoreleasePool();

		void AddObject(Ref* object);
		void Clear();
		bool Contain(Ref* obecjt);
		void Dump();

	private:
		static AutoreleasePool* sharedPool;

		std::vector<Ref*> m_Objects;
	};

}