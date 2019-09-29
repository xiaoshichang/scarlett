#include "AutoReleasePool.h"
#include "Runtime/Utils/Logging.h"

using namespace scarlett;

AutoreleasePool::AutoreleasePool() {
	m_Objects.reserve(150);
}

AutoreleasePool::~AutoreleasePool() {
	Clear();

}

void AutoreleasePool::AddObject(Ref* object) {
	m_Objects.push_back(object);
}

void AutoreleasePool::Clear() {
	std::vector<Ref*> releasings;
	releasings.swap(m_Objects);
	for (const auto &obj : releasings)
	{
		obj->Release();
	}
}

bool AutoreleasePool::Contain(Ref* object) {
	for (const auto& obj : m_Objects) {
		if (obj == object) {
			return true;
		}
	}
	return false;
}

void AutoreleasePool::Dump() {
	SCARLETT_LOG(info) << "autorelease pool dump:";
	for (const auto& obj : m_Objects) {
		SCARLETT_LOG(info) << obj << ": " << obj->GetReferenceCount();
	}
}

AutoreleasePool* AutoreleasePool::sharedPool = nullptr;

AutoreleasePool* AutoreleasePool::GetInstance() {
	if (sharedPool == nullptr) {
		sharedPool = new AutoreleasePool();
	}

	return sharedPool;
}